#include "Dma.h"
#include <Log.h>
#include <FreeRTOS.h>
#include <semphr.h>
#include <task.h>
#include <Irq.h>

extern "C" {
	void DMA1_Stream0_IRQHandler();
	void DMA1_Stream1_IRQHandler();
	void DMA1_Stream2_IRQHandler();
	void DMA1_Stream3_IRQHandler();
	void DMA1_Stream4_IRQHandler();
	void DMA1_Stream5_IRQHandler();
	void DMA1_Stream6_IRQHandler();
	void DMA1_Stream7_IRQHandler();
	void DMA2_Stream0_IRQHandler();
	void DMA2_Stream1_IRQHandler();
	void DMA2_Stream2_IRQHandler();
	void DMA2_Stream3_IRQHandler();
	void DMA2_Stream4_IRQHandler();
	void DMA2_Stream5_IRQHandler();
	void DMA2_Stream6_IRQHandler();
	void DMA2_Stream7_IRQHandler();
};

xSemaphoreHandle dmaSem[8][2];

DmaStream::DmaStream(int dmaId, int streamId, int channel): streamId(streamId), dmaId(dmaId) {
	vSemaphoreCreateBinary(dmaSem[dmaId][streamId]);
	xSemaphoreTake(dmaSem[dmaId][streamId], 0);
	if(dmaId == 1) RCC->AHB1ENR |= 1 << 21;
	else if(dmaId == 2) RCC->AHB1ENR |= 1 << 22;
	else while(1);

	if(dmaId==1)
		dma = DMA1;
	else if(dmaId==2)
		dma = DMA2;
	else while(1);
	switch(streamId) {
		case 0: case 1: case 2: case 3:
		case 4: case 5: case 6: case 7:
			stream = (DMA_Stream_TypeDef*)( (char*)dma + 0x10 + 0x18 * streamId);
			break;
		default:
			while(1);
			break;
	}

	currentBuf = 0;
	//Set channel for this stream
	stream->CR &= ~DMA_SxCR_CHSEL;
	channel &= 7;
	stream->CR |= channel << 25;

	Irq(irqNr()).enable();
	stream->CR |= DMA_SxCR_TCIE;
}

int DmaStream::irqNr() {
	if(dmaId == 1) {
		if(streamId <= 6)
			return 11 + streamId;
		if(streamId == 7)
			return DMA1_Stream7_IRQn;
		while(1);
	} else if(dmaId == 2) {
		if(streamId <= 4)
			return DMA2_Stream0_IRQn + streamId;
		if(streamId <= 7)
			return DMA2_Stream5_IRQn + streamId-5;
		while(1);
	} else while(1);
		
	return 0;
}

DmaStream& DmaStream::setCurrent(bool first) {
	if(first)
		stream->CR &= ~DMA_SxCR_CT;
	else
		stream->CR |= DMA_SxCR_CT;

	return *this;
}

DmaStream& DmaStream::peripheralFixed(bool fixed) {
	if(!fixed)
		stream->CR |= DMA_SxCR_PINC;
	else
		stream->CR &= ~DMA_SxCR_PINC;

	return *this;
}

DmaStream& DmaStream::memoryFixed(bool fixed) {
	if(!fixed)
		stream->CR |= DMA_SxCR_MINC;
	else
		stream->CR &= ~DMA_SxCR_MINC;

	return *this;
}

DmaStream& DmaStream::setDirection(direction d) {
	stream->CR &= ~DMA_SxCR_DIR;
	switch(d) {
		case P2M:
			stream->CR |= 0<<6;
			break;
		case M2P:
			stream->CR |= 1<<6;
			break;
		case M2M:
			stream->CR |= 2<<6;
			break;
		default:
			while(1);
			break;
	}

	return *this;
}

DmaStream& DmaStream::peripheralControlled(bool p) {
	if(p)
		stream->CR |= DMA_SxCR_PFCTRL;
	else
		stream->CR &= ~DMA_SxCR_PFCTRL;

	return *this;
}

DmaStream& DmaStream::numberOfData(int n) {
	n &= 0xffff;
	stream->NDTR = n;

	return *this;
}

DmaStream& DmaStream::enable() {
	setCurrent(currentBuf == 0);
	if(streamId < 4)
		if(streamId < 2)
			dma->LIFCR = 0x1f << (6*streamId);
		else
			dma->LIFCR = 0x1f << (6*streamId+4);
	else
		if(streamId < 6)
			dma->HIFCR = 0x1f << (6*(streamId-4));
		else
			dma->HIFCR = 0x1f << (6*(streamId-4)+4);
	dma->HIFCR = 0xffffffff;
	dma->LIFCR = 0xffffffff;

	stream->CR |= DMA_SxCR_EN;

	return *this;
}

DmaStream& DmaStream::wait() {
	if(!(stream->CR & DMA_SxCR_EN))
		return *this;
	xSemaphoreTake(dmaSem[dmaId][streamId], portMAX_DELAY);
	while(stream->CR & DMA_SxCR_EN);

	return *this;
}

DmaStream& DmaStream::setPeripheral(volatile void* p) {
	stream->PAR = (uint32_t)p;

	return *this;
}

DmaStream& DmaStream::setMemory(volatile void* p) {
	currentBuf = !currentBuf;
	if(currentBuf == 0)
		stream->M0AR = (uint32_t)p;
	else
		stream->M1AR = (uint32_t)p;

	return *this;
}

DmaStream& DmaStream::fifo(bool enabled) {
	if(enabled)
		stream->FCR |= DMA_SxFCR_DMDIS;
	else
		stream->FCR &= ~DMA_SxFCR_DMDIS;

	return *this;
}

static void irq_handler(DMA_TypeDef *dmab, int dma, int stream) {
	long v;
	xSemaphoreGiveFromISR(dmaSem[dma][stream], &v);
	portEND_SWITCHING_ISR(v);
	dmab->HIFCR = 0xffffffff;
	dmab->LIFCR = 0xffffffff;
}

void DMA1_Stream0_IRQHandler() { irq_handler(DMA1, 1, 0); }
void DMA1_Stream1_IRQHandler() { irq_handler(DMA1, 1, 1); }
void DMA1_Stream2_IRQHandler() { irq_handler(DMA1, 1, 2); }
void DMA1_Stream3_IRQHandler() { irq_handler(DMA1, 1, 3); }
void DMA1_Stream4_IRQHandler() { irq_handler(DMA1, 1, 4); }
void DMA1_Stream5_IRQHandler() { irq_handler(DMA1, 1, 5); }
void DMA1_Stream6_IRQHandler() { irq_handler(DMA1, 1, 6); }
void DMA1_Stream7_IRQHandler() { irq_handler(DMA1, 1, 7); }

void DMA2_Stream0_IRQHandler() { irq_handler(DMA2, 2, 0); }
void DMA2_Stream1_IRQHandler() { irq_handler(DMA2, 2, 1); }
void DMA2_Stream2_IRQHandler() { irq_handler(DMA2, 2, 2); }
void DMA2_Stream3_IRQHandler() { irq_handler(DMA2, 2, 3); }
void DMA2_Stream4_IRQHandler() { irq_handler(DMA2, 2, 4); }
void DMA2_Stream5_IRQHandler() { irq_handler(DMA2, 2, 5); }
void DMA2_Stream6_IRQHandler() { irq_handler(DMA2, 2, 6); }
void DMA2_Stream7_IRQHandler() { irq_handler(DMA2, 2, 7); }
