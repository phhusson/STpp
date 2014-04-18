#include "Dma.h"
#include <Log.h>

DmaStream::DmaStream(int dmaId, int streamId, int channel): streamId(streamId) {
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
		dma->LIFCR &= ~(0x1f << (6*streamId));
	else
		dma->HIFCR &= ~(0x1f << (6*(streamId-4)));

	stream->CR |= DMA_SxCR_EN;

	return *this;
}

DmaStream& DmaStream::wait() {
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
