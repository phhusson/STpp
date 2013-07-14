class GpioPort;
class Gpio {
	private:
		GpioPort *port;
		Gpio(GpioPort, int);
		friend class GpioPort;
	public:
		enum Direction {
			INPUT,
			OUTPUT
		};
		void setDirection(Gpio::Direction);

		enum Speed {
			SPEED_50MHz,
			SPEED_10MHz,
			SPEED_2MHz
		};
		void setSpeed(Gpio::Speed);
		void setPushPull();
		void setOpenDrain();

		enum Function {
			GPIO
		};
		void setFunction(Gpio::Function);

		void setState(bool);
		bool getState();
};

class GpioPort {
	private:
		volatile void *base;
	public:
		Gpio operator[](int n) {
			return Gpio(this, n);
		}
		GpioPort(volatile void* b) : base(b) {};
		friend class Gpio;
};

extern GpioPort GPIOA;
extern GpioPort GPIOB;
extern GpioPort GPIOC;
extern GpioPort GPIOD;
extern GpioPort GPIOE;
extern GpioPort GPIOF;
extern GpioPort GPIOG;
extern GpioPort GPIOH;
extern GpioPort GPIOI;
