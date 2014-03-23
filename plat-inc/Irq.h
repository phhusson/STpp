#ifndef _IRQ_H
#define _IRQ_H

class Irq {
	private:
		int irq;
	public:
		Irq(int n);
		Irq& enable();
		Irq& disable();
		Irq& setPriority(int ip);
};
#endif /* IRQ_H */
