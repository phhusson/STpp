#ifndef _GPIO_H
#define _GPIO_H
#include <stm32f4xx.h>

class GpioPort;
class Gpio {
	public:
		Gpio();
		Gpio& operator=(Gpio&);

		enum Direction {
			INPUT,
			OUTPUT,
			ANALOG
		};
		Gpio& setDirection(Gpio::Direction);

		enum Speed {
			SPEED_100MHz,
			SPEED_50MHz,
			SPEED_25MHz,
			SPEED_2MHz
		};
		Gpio& setSpeed(Gpio::Speed);
		Gpio& setPushPull();
		Gpio& setOpenDrain();

		enum Function {
			GPIO,
			ALTERNATE
		};
		Gpio& setFunction(Gpio::Function);

		Gpio& setState(bool);
		bool getState();
		enum Resistor {
			NONE,
			PULL_UP,
			PULL_DOWN,
		};
		Gpio& setResistor(Resistor);
		enum AF {
			SYSTEM,
			TIM1_2,
			TIM3_5,
			TIM8_11,
			I2C1_3,
			SPI1_2,
			SPI_3,
			USART1_3,
			USART4_6,
			CAN1_2_TIM12_14,
			OTG_FS_HS,
			Eth,
			FSMC_SDIO_OTG_HS,
			Dcmi,
			AF_14,
			EVENT_OUT,
		};
		Gpio& setAlternate(int);
		Gpio& setAlternate(AF);
		inline operator bool() { return getState(); };
	private:
		GpioPort *port;
		int number;
		Gpio::Function function;
		Gpio::Direction direction;
		Gpio(GpioPort*, int);
		void updateModeR();
		friend class GpioPort;
		friend class Exti;
};

class GpioPort {
	private:
		volatile GPIO_TypeDef *base;
	public:
		Gpio operator[](int n) {
			return Gpio(this, n);
		}
		GpioPort(volatile GPIO_TypeDef* b);
		friend class Gpio;
		int getPortNumber();
};

extern GpioPort GpioA;
extern GpioPort GpioB;
extern GpioPort GpioC;
extern GpioPort GpioD;
extern GpioPort GpioE;
extern GpioPort GpioF;
extern GpioPort GpioG;
extern GpioPort GpioH;
extern GpioPort GpioI;
inline bool Gpio::getState() { return !!(port->base->IDR&(1<<number)); };

#endif /* _GPIO_H */
