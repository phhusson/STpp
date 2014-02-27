USB_OTG_BASE=3rdparty/STM32_USB-Host-Device_Lib_V2.1.0/Libraries/STM32_USB_OTG_Driver/
USB_OTG_INC=-I$(USB_OTG_BASE)/inc
USB_OTG_SRC=$(USB_OTG_BASE)/src/usb_core.c
USB_OTG_SRC+=$(USB_OTG_BASE)/src/usb_dcd.c
USB_OTG_SRC+=$(USB_OTG_BASE)/src/usb_dcd_int.c
USB_OTG_SRC+=$(USB_OTG_BASE)/src/usb_hcd.c
USB_OTG_SRC+=$(USB_OTG_BASE)/src/usb_hcd_int.c

USB_DEV_BASE=3rdparty/STM32_USB-Host-Device_Lib_V2.1.0/Libraries/STM32_USB_Device_Library/
USB_DEV_INC=-I$(USB_DEV_BASE)/Core/inc
USB_DEV_INC+=-I$(USB_DEV_BASE)/Class/hid/inc
USB_DEV_INC+=-I$(USB_DEV_BASE)/Class/msc/inc
USB_DEV_INC+=-I$(USB_DEV_BASE)/Class/cdc/inc
USB_DEV_SRC=$(USB_DEV_BASE)/Core/src/usbd_core.c
USB_DEV_SRC+=$(USB_DEV_BASE)/Core/src/usbd_ioreq.c
USB_DEV_SRC+=$(USB_DEV_BASE)/Core/src/usbd_req.c

USB_DEV_SRC+=$(USB_DEV_BASE)/Class/hid/src/usbd_hid_core.c
USB_DEV_SRC+=$(USB_DEV_BASE)/Class/cdc/src/usbd_cdc_core.c

USB_HOST_BASE=3rdparty/STM32_USB-Host-Device_Lib_V2.1.0/Libraries/STM32_USB_HOST_Library/
USB_HOST_INC=-I$(USB_HOST_BASE)/Core/inc/
USB_HOST_INC+=-I$(USB_HOST_BASE)/Class/HID/inc/
USB_HOST_INC+=-I$(USB_HOST_BASE)/Class/MSC/inc/
USB_HOST_SRC=$(USB_HOST_BASE)/Core/src/usbh_core.c
USB_HOST_SRC+=$(USB_HOST_BASE)/Core/src/usbh_hcs.c
USB_HOST_SRC+=$(USB_HOST_BASE)/Core/src/usbh_ioreq.c
USB_HOST_SRC+=$(USB_HOST_BASE)/Core/src/usbh_stdreq.c

USB_INC=$(USB_OTG_INC) $(USB_DEV_INC) $(USB_HOST_INC)
USB_SRC=$(USB_OTG_SRC) $(USB_DEV_SRC) $(USB_HOST_SRC)
USB_OBJS=$(subst .c,.o,$(USB_SRC))
