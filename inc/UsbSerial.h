#ifndef __USB_SERIAL_H
#define __USB_SERIAL_H

#include <OStream.h>
#include <IStream.h>
#include <Usb.h>

class UsbSerial : public OStream, public IStream {
	private:
		Usb u;
	public:
		UsbSerial();
		//OStream
		virtual UsbSerial& put(char);
		virtual UsbSerial& endl();

		//IStream
		virtual int get();
		virtual bool available();
};
#endif

