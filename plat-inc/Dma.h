#ifndef _DMA_H
#define _DMA_H

#include <stm32f4xx.h>

class DmaStream {
	private:
		DMA_TypeDef* dma;
		DMA_Stream_TypeDef* stream;
		int currentBuf;
		int streamId;
		DmaStream& setCurrent(bool first);

	public:
		DmaStream(int dmaController, int streamNumber, int channel);
		DmaStream& peripheralFixed(bool fixed = true);
		DmaStream& memoryFixed(bool fixed = true);
		enum direction {
			P2M,
			M2P,
			M2M
		};
		DmaStream& setDirection(direction);
		DmaStream& peripheralControlled(bool p=true);
		DmaStream& numberOfData(int);

		DmaStream& enable();
		DmaStream& wait();
		DmaStream& setPeripheral(volatile void*);
		DmaStream& setMemory(volatile void*);
};

#endif /* _DMA_H */
