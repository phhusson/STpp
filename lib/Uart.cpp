#include <FreeRTOS.h>
#include <queue.h>
#include <Uart.h>
#include <Log.h>
#include <Irq.h>
#include <stm32f4xx.h>

extern "C" {
	void USART1_IRQHandler(void);
	void USART2_IRQHandler(void);
	void USART3_IRQHandler(void);
	void UART4_IRQHandler(void);
	void UART5_IRQHandler(void);
	void USART6_IRQHandler(void);
};

static xQueueHandle uart_queue_rx[7];
//static xQueueHandle uart_queue_tx[7];

static void irq_handler(volatile USART_TypeDef* b, int i) {
	if(b->SR & USART_SR_RXNE) {
		char val = b->DR;
		xQueueSendFromISR(uart_queue_rx[i], &val, NULL);
	}
}

void USART1_IRQHandler() {
	irq_handler(USART2, 1);
}

void USART2_IRQHandler() {
	irq_handler(USART2, 2);
}

void USART3_IRQHandler() {
	irq_handler(USART3, 3);
}

void UART4_IRQHandler() {
	irq_handler(UART4, 4);
}

void UART5_IRQHandler() {
	irq_handler(UART5, 5);
}

void USART6_IRQHandler() {
	irq_handler(USART6, 6);
}

Uart::Uart(int n) :
	number(n) {
	switch(n) {
		case 1:
			RCC->APB2ENR |= 1<<4;
			base = (volatile USART_TypeDef*)0x40011000;
			break;
		case 2:
		case 3:
		case 4:
		case 5:
			RCC->APB1ENR |= 1<<(15+n);
			base = (volatile USART_TypeDef*)(0x40004400+(n-2)*0x400);
			break;
		case 6:
			RCC->APB2ENR |= 1<<5;
			base = (volatile USART_TypeDef*)0x40011400;
			break;
	}
	uart_queue_rx[n] = xQueueCreate(32, 1);
}

Uart& Uart::configGpio(Gpio& p) {
	int af;

	if(number < 4)
		af = 7;
	else
		af = 8;

	p.setAlternate(af);
	p.setSpeed(Gpio::SPEED_100MHz);

	//Perhaps control direction... one day...
	return *this;
}

Uart& Uart::put(char c) {
	while(! (base->SR & (1<<7)));
	base->DR = c;
	while(! (base->SR & (1<<7)));

	return *this;
}

Uart& Uart::setMantissa(int m) {
	m&=0xfff;
	base->BRR = (base->BRR & ~(0xfff<<4)) | (m<<4);
	return *this;
}

Uart& Uart::setFraction(int f) {
	f&=0xf;
	base->BRR = (base->BRR & ~(0xf)) | f;
	return *this;
}

Uart& Uart::enableReceive() {
	base->CR1 |= 1<<2;
	return *this;
}

Uart& Uart::disableReceive() {
	base->CR1 &= ~(1<<2);
	return *this;
}

Uart& Uart::enableTransmitter() {
	base->CR1 |= 1<<3;
	return *this;
}

Uart& Uart::disableTransmitter() {
	//Wait for transmission complete
	while(! (base->SR & (1<<6)));

	base->CR1 &= ~(1<<3);
	return *this;
}

Uart& Uart::enable() {
#if 1
	//Enable IRQ
	//On TX
	//base->CR1 |= 1<<6;
	//On RX
	base->CR1 |= 1<<5;

	Irq(USART2_IRQn)
		.setPriority(230)
		.enable();
#endif

	base->CR1 |= 1<<13;
	return *this;
}

Uart& Uart::disable() {
	base->CR1 &= ~(1<<13);
	return *this;
}

char Uart::waitForNext() {
	char c;
	xQueueReceive(uart_queue_rx[number], &c, portMAX_DELAY);
	return c;
}

Uart& Uart::sendBreak() {
	base->CR1 |= 1;
	while( base->CR1 & 1);
	return *this;
}

Uart& Uart::setHalfDuplex(bool b) {
	if(b) {
		base->CR3 |= 1<<3;
	} else {
		base->CR3 &= ~(1<<3);
	}
	return *this;
}

Uart& Uart::endl() {
	put('\r');
	put('\n');
}
