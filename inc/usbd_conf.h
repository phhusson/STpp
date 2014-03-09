#ifndef _USBD_CONF_H
#define _USBD_CONF_h

#define USBD_CFG_MAX_NUM           1
#define USBD_ITF_MAX_NUM           1
#define USB_MAX_STR_DESC_SIZ       64 
#define USBD_SELF_POWERED               
#define USBD_DYNAMIC_DESCRIPTOR_CHANGE_ENABLED

#define HID_IN_EP                    0x81
#define HID_OUT_EP                   0x01

#define HID_IN_PACKET                4
#define HID_OUT_PACKET               4

#define MSC_IN_EP	0x81
#define MSC_OUT_EP	0x01
#define MSC_MAX_PACKET 16

#define APP_FOPS	UsbSerial_fops
#define CDC_IN_EP	0x81
#define CDC_OUT_EP	0x01
#define CDC_CMD_EP	0x82
#define CDC_DATA_MAX_PACKET_SIZE 64
#define APP_RX_DATA_SIZE 1024
#define CDC_CMD_PACKET_SZE 8
#define CDC_IN_FRAME_INTERVAL 5

#endif /* _USBD_CONF_H */
