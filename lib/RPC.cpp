#include "RPC.h"
#include <Tasks.h>
#include <Log.h>

RPC::RPC(): in(NULL), out(NULL) {
}

RPC& RPC::setStream(IStream* in, OStream* out) {
	this->in = in;
	this->out = out;
	return *this;
}

void RPC::waitIngoing() {
	log << "Ingoing thread started" << endl;
	while(true) {
		do {
			in->wait();
			log << "Received something" << endl;
		} while(!running.tryTake());
		log << "Notify main thread" << endl;
		ingoing = true;
		signalIncoming.give();
	}
}

void RPC::handleIncoming() {
	unsigned char c;
	unsigned char id;

	log << "Received packet" << endl;

	*in >> c;
	//Not a start of frame
	if(c != 0xa5)
			  return;
	*in >> id;
	if(id>sizeof(cbs))
			  return;

	unsigned char checksum = 0;
	char buf[128];
	unsigned char len;
	*in >> len;
	if(len > sizeof(buf)) {
		return;
	}

	for(int i=0; i<len; ++i) {
		*in >> buf[i];
		checksum+=buf[i];
	}
	*in >> c;
#if 0
	if(c != (unsigned char)(~checksum)) {
		//NACK bad checksum
		*out << (char) 0xa5 << (char) 0xf2;
		return;
	}
#endif

	if(cbs[id]) {
		cbs[id](len, buf);
		//ACK
		*out << (char) 0xa5 << (char) 0xf0;
	} else {
		//NACK not found
		*out << (char) 0xa5 << (char) 0xf1;
	}
}

void RPC::handleOutgoing() {
}

void RPC::runLogic() {
	signalIncoming.tryTake();
	while(true) {
		running.give();
		ingoing = false;

		log << "Waiting for signal" << endl;
		signalIncoming.take();

		log << "Got a signal" << endl;
		if(ingoing) {
			log << "Incoming signal" << endl;
			handleIncoming();
		} else {
			log << "Outgoing signal" << endl;
			handleOutgoing();
		}
	}
}

void RPC::run() {
	Task Ingoing([this]() { waitIngoing();}, "Ingoing RPC", 128);
	runLogic();
}

RPC& RPC::registerClass(int id, Callback cb) {
		  cbs[id] = cb;
		  return *this;
}
