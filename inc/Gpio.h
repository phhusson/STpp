class GpioPort;
class Gpio {
	private:
		GpioPort *port;
		int number;
		Gpio(GpioPort*, int);
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

extern GpioPort GpioA;
extern GpioPort GpioB;
extern GpioPort GpioC;
extern GpioPort GpioD;
extern GpioPort GpioE;
extern GpioPort GpioF;
extern GpioPort GpioG;
extern GpioPort GpioH;
extern GpioPort GpioI;
