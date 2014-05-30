#include <Board.h>
#include <Bluetooth.h>
#include <Gpio.h>
#include <Exti.h>
#include <Irq.h>
#include <Tasks.h>
#include <Timer.h>
#include <Log.h>
#include <Lock.h>
#include <IRRemote.h>

int digit_read(int low, int high) {
	int v = 0;
	for(int i=3; i>=0; --i) {
		v <<= 1;
		if(high & (0xff<< (8*i)))
			v |= 1;
	}
	for(int i=3; i>=0; --i) {
		v <<= 1;
		if(low & (0xff<< (8*i)))
			v |= 1;
	}
	if(v & 0x80) {
		//log << "Dotted ";
		v&= 0x7f;
	}
	if(v == 0) {
		return 0;
	} else if(v == 0x5f) {
		return 0;
	} else if(v == 0x06) {
		return 1;
	} else if(v == 0x6b) {
		return 2;
	} else if(v == 0x2F) {
		return 3;
	} else if(v == 0x36) {
		return 4;
	} else if(v == 0x3d) {
		return 5;
	} else if(v == 0x7d) {
		return 6;
	} else if(v == 0x17) {
		return 7;
	} else if(v == 0x7f) {
		return 8;
	} else if(v == 0x3f) {
		return 9;
	} else {
		return -1;
	}
}

int main() {
	//Not actually a clock
	//But a non-displayed digit that is used as a clock
	auto clk = GpioB[5];
	auto data9 = GpioB[9];
	auto data8 = GpioB[8];
	auto data7 = GpioB[7];
	auto data6 = GpioB[6];

	auto Pwm1_g = GpioA[1];
	Pwm pompe1(Pwm1_g, Tim2, 2);

	auto Pwm2_g = GpioA[2];
	Pwm pompe2(Pwm2_g, Tim2, 3);

	auto Pwm3_g = GpioA[3];
	Pwm pompe3(Pwm3_g, Tim2, 4);

#if 0
	//Usart3
	auto BT_Tx = GpioB[10];
	auto BT_Rx = GpioB[11];

	log << "Starting..." << endl;

	Bluetooth bt(3, BT_Tx, BT_Rx);
	//0x105f .. 0x1230
	//= 0x1147
	bt.setMantissa(0x114).setFraction(0x7);
	while(1) {
		for(int i=0x105F; i<0x1230; ++i) {
			bt.setMantissa(i >> 4).setFraction(i & 0xf);
			if(bt.available()) {
				bt << (int) i << endl;
				while(bt.available()) bt.get();
			}
			time.msleep(50);
		}
	}
#endif

#define prepare_gpio_input(g) { \
	g \
		.setDirection(Gpio::INPUT) \
		.setSpeed(Gpio::SPEED_100MHz) \
		.setResistor(Gpio::PULL_DOWN) \
		.setFunction(Gpio::GPIO); \
}

	prepare_gpio_input(clk);
	prepare_gpio_input(data6);
	prepare_gpio_input(data7);
	prepare_gpio_input(data8);
	prepare_gpio_input(data9);



	volatile int rawValue = 0;
	volatile int lastUpdate = 0;

	Tim4
	    //(30.66*8)Hz = 245.28Hz
	    //42MHz / 245.28Hz = 171 478 ~= 73 * 2349
	    .setPrescaler(72)
	    .setAutoReload(2349)
	    .setUIE(true)
	    .setURS(true)
	    .setOneShot(false);


	//Decode the LCD
	Tim4
	    .setTopCB([&rawValue, &lastUpdate, &clk, &data9, &data8, &data7, &data6](int timer_id) {
		(void) timer_id;
		static int v8 = 0, v9 = 0, v7 = 0, v6 = 0;
		static int pos = 0;
		static int firstValue = -42;

		bool a = clk;
		bool d9 = data9;
		bool d8 = data8;
		bool d7 = data7;
		bool d6 = data6;

		if(a) {
			v9 <<= 4;
			v8 <<= 4;
			v7 <<= 4;
			v6 <<= 4;
			if(d9)
				v9 |= 0xf;
			if(d8)
				v8 |= 0xf;
			if(d7)
				v7 |= 0xf;
			if(d6)
				v6 |= 0xf;
			pos ++;
		}
		if(!a && pos == 8) {
			//We are reading the 1-if-off cycle so invert
			v9 = ~v9;
			int n9 = 0;
			for(int i=0; i<4; ++i) {
				if( v9 & (0xff<<(i*8)))
					++n9;
			}

			//We are reading the 1-if-off cycle so invert
			v8 = ~v8;
			int n8 = 0;
			for(int i=0; i<4; ++i) {
				if( v8 & (0xff<<(i*8)))
					++n8;
			}

			//We are reading the 1-if-off cycle so invert
			v7 = ~v7;
			int n7 = 0;
			for(int i=0; i<4; ++i) {
				if( v7 & (0xff<<(i*8)))
					++n7;
			}

			//We are reading the 1-if-off cycle so invert
			v6 = ~v6;
			int n6 = 0;
			for(int i=0; i<4; ++i) {
				if( v6 & (0xff<<(i*8)))
					++n6;
			}

			int t1 = digit_read(v7, v6);
			int t2 = digit_read(v9, v8);
			if(t1 != -1 && t2 != -1 ) {
				int tmpValue = t1;
				tmpValue *= 10;
				tmpValue += t2;
				rawValue = tmpValue;
				if(firstValue == -42)
					firstValue = rawValue;
			}
#if 0
			//Debug balance wires
			log
				<< ", v6 = " << v6
				<< ", v7 = " << v7
				<< ", v8 = " << v8
				<< ", v9 = " << v9
				<< ", t1 = " << t1
				<< ", t2 = " << t2
				<< endl;
#endif

			lastUpdate = xTaskGetTickCountFromISR();

			v9 = 0;
			v8 = 0;
			v7 = 0;
			v6 = 0;
			pos = 0;
		} else if ( (!a && pos) ) {
			//Don't display if it's 7, it just means the clock is a bit offseted
			//(Just hope not all values are so)
			if(pos != 7) 
				log << "Failed ! " << pos << ":" << v6 << endl;
			pos = 0;
		}
	})
		.enable();
	
	Irq(Tim4.irqNr())
		.setPriority(15)
		.enable();

	Pwm* currentPump = &pompe1;
	int currentAim = 100;
	typedef enum {
		WATER,
		SYRUP,
	} liquidType;

	liquidType currentLiquid = WATER;

	BinarySemaphore newCommand;
	BinarySemaphore commandDone;

	//Ensure commandDone is false
	commandDone.tryTake();
	newCommand.tryTake();

	Task asserv([&rawValue, &lastUpdate, &currentPump, &currentAim, &currentLiquid, &newCommand, &commandDone]() {
		static const int liquidValues[][5] = {
			//Water
			{ 100, 80, 30, 0},
			//Syrup, always 3 extra cl
			{ 150, 100, 70, 30},
		};
		int previousUpdate = 0;
		int firstValue = -42;
		int currentValue = 0;

		newCommand.take();
		while(1) {
			if(lastUpdate > previousUpdate) {
				previousUpdate = lastUpdate;

				if(firstValue != -42) {
					{
						//Compute new position
						int tValue = currentValue/100;
						tValue *= 100;
						tValue += rawValue;
						//If < -= 3cl, must mean += 7cl
						if(tValue < (currentValue-30)) {
							log << "tValue = " << tValue << ", value = " << currentValue << endl;
							tValue += 100;
						}
						//If > += 7cl, must mean -=3cl
						if(tValue > (currentValue+70)) {
							log << "tValue = " << tValue << ", value = " << currentValue << endl;
							tValue -= 100;
						}
						currentValue = tValue;
					}

					{
						//"Asserv"
						int actualValue = currentValue - firstValue;

						int delta = currentAim - actualValue;

						if(delta > liquidValues[currentLiquid][0]) {
							currentPump->setDutyCycle(100);
							log << "pwm = " << 100 << endl;
						} else if(delta > liquidValues[currentLiquid][1]) {
							currentPump->setDutyCycle(50);
							log << "pwm = " << 50 << endl;
						} else if(delta > liquidValues[currentLiquid][2] || actualValue < 5) {
							currentPump->setDutyCycle(35);
							log << "pwm = " << 35 << endl;
						} else if(delta > liquidValues[currentLiquid][3]) {
							currentPump->setDutyCycle(25);
							log << "pwm = " << 25 << endl;
						} else {
							log << "pwm = " << 0 << endl;
							currentPump->setDutyCycle(0);
							commandDone.give();
							newCommand.take();

							firstValue = rawValue;
							currentValue = rawValue;
						}

					}
				} else {
					if(rawValue == 88 || rawValue == 0) {
						time.msleep(3000);
					}
					firstValue = rawValue;
				}
			}
			time.msleep(1);
		}
	}, "Asserv");

#if 0
	currentAim = 30;
	currentPump = &pompe1;
	newCommand.give();
	commandDone.take();

	currentLiquid = SYRUP;
	currentAim = 20;
	currentPump = &pompe2;
	newCommand.give();
	commandDone.take();

	time.msleep(1000);
	currentAim = 200;
	currentPump = &pompe3;
	newCommand.give();
	commandDone.take();
#endif

	log << "Starting IR Remote" << endl;
	auto irpin = GpioE[7];
	IRRemote remote(Tim12, irpin);

	log << "IR Remote started" << endl;

	while(1) {
		LedG.setDutyCycle(100);
		int cmd = remote.next();
		LedG.setDutyCycle(0);
		log << "Got cmd = " << cmd << endl;
		switch(cmd) {
			case 0:
				//10cl pompe 1
				currentAim = 100;
				currentPump = &pompe1;
				newCommand.give();
				commandDone.take();
				break;
			case 1:
				//15cl pompe 1
				currentAim = 150;
				currentPump = &pompe1;
				newCommand.give();
				commandDone.take();
				break;
			case 2:
				//10cl pompe 1 + 10 cl pompe 3
				currentAim = 100;
				currentPump = &pompe1;
				newCommand.give();
				commandDone.take();

				currentAim = 100;
				currentPump = &pompe3;
				newCommand.give();
				commandDone.take();
				break;

			case 3:
				//3cl pompe 1 + 15 cl pompe 3
				currentAim = 30;
				currentPump = &pompe1;
				newCommand.give();
				commandDone.take();

				currentAim = 150;
				currentPump = &pompe3;
				newCommand.give();
				commandDone.take();
				break;
			default:
				//Bite.
				break;
		};
	}
}
