#ifndef __USB_SERIAL_H
#define __USB_SERIAL_H

#include <OStream.h>
#include <Usb.h>

class UsbSerial : public OStream {
	private:
		Usb u;
	public:
		UsbSerial();
		virtual UsbSerial& put(char);
		virtual UsbSerial& endl();
};
#endif

