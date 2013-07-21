#ifndef _USB_CONF_H
#define _USB_CONF_H

#include <stm32f4xx.h>
#define __ALIGN_BEGIN
#define __ALIGN_END

#define USE_USB_OTG_FS
#define USB_OTG_FS_CORE

//Device
#define RX_FIFO_FS_SIZE 128
#define TX0_FIFO_FS_SIZE 64
#define TX1_FIFO_FS_SIZE 128
#define TX2_FIFO_FS_SIZE 9
#define TX3_FIFO_FS_SIZE 0

//Host
#define TXH_NP_FS_FIFOSIZ 96
#define TXH_P_FS_FIFOSIZ 96

#define VBUS_SENSING_ENABLED
#define USE_DEVICE_MODE
//#define USE_OTG_MODE
#define USE_HOST_MODE
#define __packed    __attribute__ ((__packed__))

#endif /* _USB_CONF_H */
