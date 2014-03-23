#include <FreeRTOS.h>
#include <queue.h>
#include "Log.h"
#include "UsbSerial.h"

extern "C" {
#include "usbd_cdc_core.h"
#include "usbd_conf.h"
};

typedef struct
{
  uint32_t bitrate;
  uint8_t  format;
  uint8_t  paritytype;
  uint8_t  datatype;
}LINE_CODING;

LINE_CODING linecoding = { 115200, 0, 0, 8 };

/* These are external variables imported from CDC core to be used for IN 
   transfer management. */
extern volatile uint8_t  APP_Rx_Buffer []; /* Write CDC received data in this buffer.
                                     These data will be sent over USB IN endpoint
                                     in the CDC core functions. */
extern volatile uint32_t APP_Rx_ptr_in;    /* Increment this pointer or roll it back to
                                     start address when writing received data
                                     in the buffer APP_Rx_Buffer. */

static uint16_t UsbSerial_Init(void)
{
  return USBD_OK;
}

static uint16_t UsbSerial_DeInit(void)
{
  return USBD_OK;
}

static uint16_t UsbSerial_Ctrl (uint32_t Cmd, uint8_t* Buf, uint32_t Len) { 
	switch (Cmd) {
		case SET_LINE_CODING:
			linecoding.bitrate = (uint32_t)(Buf[0] | (Buf[1] << 8) | (Buf[2] << 16) | (Buf[3] << 24));
			linecoding.format = Buf[4];
			linecoding.paritytype = Buf[5];
			linecoding.datatype = Buf[6];
			break;

		case GET_LINE_CODING:
			Buf[0] = (uint8_t)(linecoding.bitrate);
			Buf[1] = (uint8_t)(linecoding.bitrate >> 8);
			Buf[2] = (uint8_t)(linecoding.bitrate >> 16);
			Buf[3] = (uint8_t)(linecoding.bitrate >> 24);
			Buf[4] = linecoding.format;
			Buf[5] = linecoding.paritytype;
			Buf[6] = linecoding.datatype; 
			break;

		case SET_CONTROL_LINE_STATE:
		case SEND_BREAK:
		case SEND_ENCAPSULATED_COMMAND:
		case GET_ENCAPSULATED_RESPONSE:
		case SET_COMM_FEATURE:
		case GET_COMM_FEATURE:
		case CLEAR_COMM_FEATURE:
		default:
			break;
	}

	return USBD_OK;
}

static xQueueHandle rx_queue;
static uint16_t UsbSerial_DataRx (uint8_t* Buf, uint32_t Len) {
	uint32_t i;

	for (i = 0; i < Len; i++)
		xQueueSendFromISR(rx_queue, Buf+i, NULL);

	return USBD_OK;
}

CDC_IF_Prop_TypeDef UsbSerial_fops = 
{
	UsbSerial_Init,
	UsbSerial_DeInit,
	UsbSerial_Ctrl,
	NULL,
	UsbSerial_DataRx
};

extern USB_OTG_CORE_HANDLE USB_OTG_dev;
extern USBD_DEVICE USR_desc;
extern USBD_Usr_cb_TypeDef USR_cb;

UsbSerial::UsbSerial() {
	USBD_Init(&USB_OTG_dev,
			USB_OTG_FS_CORE_ID,
			&USR_desc,
			&USBD_CDC_cb,
			&USR_cb);
	rx_queue = xQueueCreate(128, 1);
}

extern volatile uint32_t APP_Rx_ptr_out;
UsbSerial& UsbSerial::put(char c) {
	APP_Rx_Buffer[APP_Rx_ptr_in] = c;
	APP_Rx_ptr_in++;

	while(1) {
		uint32_t delta = APP_Rx_ptr_in - APP_Rx_ptr_out;
		if(delta<0)
			delta += APP_RX_DATA_SIZE;
		if(delta < APP_RX_DATA_SIZE/4)
			break;
	}

	APP_Rx_ptr_in %= APP_RX_DATA_SIZE;
	return *this;
}

UsbSerial& UsbSerial::endl() {
	put('\r');
	put('\n');
	return *this;
}

int UsbSerial::get() {
	char r;
	int res = xQueueReceive(rx_queue, &r, 0);
	if(!res)
		return -1;
	return r;
}

bool UsbSerial::available() {
	return !xQueueIsQueueEmptyFromISR(rx_queue);
}

void UsbSerial::wait() {
	char c;
	xQueuePeek(rx_queue, &c, portMAX_DELAY);
}
