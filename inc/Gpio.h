class GpioPort;
class Gpio {
	public:
		enum Direction {
			INPUT,
			OUTPUT,
			ANALOG
		};
		void setDirection(Gpio::Direction);

		enum Speed {
			SPEED_100MHz,
			SPEED_50MHz,
			SPEED_25MHz,
			SPEED_2MHz
		};
		void setSpeed(Gpio::Speed);
		void setPushPull();
		void setOpenDrain();

		enum Function {
			GPIO,
			ALTERNATE
		};
		void setFunction(Gpio::Function);

		void setState(bool);
		bool getState();
	private:
		GpioPort *port;
		int number;
		Gpio::Function function;
		Gpio::Direction direction;
		Gpio(GpioPort*, int);
		void updateModeR();
		friend class GpioPort;
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
