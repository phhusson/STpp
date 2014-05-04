#ifndef _RPC_H
#define _RPC_H

#include <IStream.h>
#include <OStream.h>
#include <Lock.h>
#include <tr1/functional>

class RPC {
	private:
		typedef std::tr1::function<void(int,char*)> Callback;
		//IDs goes from 0x00 to 0x20
		Callback cbs[0x20];

		BinarySemaphore signalIncoming, running;
		volatile bool ingoing;
	public:
		IStream* in;
		OStream* out;

		RPC();
		RPC& setStream(IStream* in, OStream *out);
		RPC& registerClass(int, Callback);
		void run();
	private:
		void handleIncoming();
		void handleOutgoing();
		void runLogic();
		void waitIngoing();
};

#endif /* _RPC_H */
