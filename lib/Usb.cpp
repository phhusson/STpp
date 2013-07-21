#include <stm32f4xx.h>
#include <Board.h>
#include <Irq.h>
#include <Usb.h>
#include <Log.h>
#include <Exti.h>

extern "C" {
#include <usb_defines.h>
#include <usb_core.h>
#include <usbd_core.h>
#include <usbd_hid_core.h>
};

extern "C" {
	USB_OTG_CORE_HANDLE  USB_OTG_dev;
	extern USBD_DEVICE USR_desc;
	void USB_OTG_BSP_Init(void);
	void USB_OTG_BSP_EnableInterrupt(void);
	void USB_OTG_BSP_DriveVBUS(uint32_t speed, uint8_t state);
	void USB_OTG_BSP_ConfigVBUS(uint32_t speed);
	void USB_OTG_BSP_uDelay (const uint32_t usec);
	void USB_OTG_BSP_mDelay (const uint32_t msec);
	void USB_OTG_BSP_TimerIRQ (void);

	extern uint32_t USBD_OTG_ISR_Handler (USB_OTG_CORE_HANDLE *pdev);
	void OTG_FS_IRQHandler(void) {
		USBD_OTG_ISR_Handler (&USB_OTG_dev);
	}
};

void USB_OTG_BSP_Init(void) {
	RCC->AHB2ENR |= 1<<7;
}

void USB_OTG_BSP_EnableInterrupt(void) {
	Irq(OTG_FS_IRQn)
		.setPriority(254)
		.enable();
}

void USB_OTG_BSP_DriveVBUS(uint32_t speed, uint8_t state) {
	USB_Vbus_en.setState(state == 0);
}

void USB_OTG_BSP_ConfigVBUS(uint32_t speed)
{

}

void USB_OTG_BSP_uDelay (const uint32_t usec)
{
#if 1
	Tim6.setAutoReload(usec*42);
	Tim6.enable();
	Tim6.wait();
	Tim6.disable();
#else
  uint32_t count = 0;
  const uint32_t utime = (120 * usec / 7);
  do
  {
    if ( ++count > utime )
    {
      return ;
    }
  }
  while (1); 
#endif
}

void USB_OTG_BSP_mDelay (uint32_t msec)
{
	while(msec) {
		USB_OTG_BSP_uDelay(1000);
		msec--;
	}
}

void USB_OTG_BSP_TimerIRQ (void)
{

} 


void USBD_USR_Init() {
	log << "USBD initing" << Log::endl;
}

void USBD_USR_DeviceReset(uint8_t speed ) {
	log << "USBD resting" << Log::endl;
}

void USBD_USR_DeviceConfigured (void) {
	log << "HID interface started" << Log::endl;
}

void USBD_USR_DeviceConnected (void) {
	log << "HID interface starting" << Log::endl;
}

void USBD_USR_DeviceDisconnected (void) {
	log << "HID interface stopped" << Log::endl;
}

void USBD_USR_DeviceSuspended() {
	log << "USB suspend" << Log::endl;
}

void USBD_USR_DeviceResumed() {
	log << "Device woke up" << Log::endl;
}

USBD_Usr_cb_TypeDef USR_cb =
{
  USBD_USR_Init,
  USBD_USR_DeviceReset,
  USBD_USR_DeviceConfigured,
  USBD_USR_DeviceSuspended,
  USBD_USR_DeviceResumed,
  
  USBD_USR_DeviceConnected,
  USBD_USR_DeviceDisconnected,  
};

static uint8_t hid_buf[4];
Usb::Usb() {

	Tim6
		.setPrescaler(1)
		.setAutoReloadBuffered(true)
		.setOneShot(true);
	log << "Tim6 configured" << Log::endl;

	USB_Vbus_en
		.setSpeed(Gpio::SPEED_100MHz)
		.setPushPull()
		.setFunction(Gpio::GPIO)
		.setDirection(Gpio::OUTPUT)
		//Enforce vbus off
		.setState(true);

	USB_DM
		.setSpeed(Gpio::SPEED_100MHz)
		.setAlternate(Gpio::OTG_FS_HS);
	USB_DP
		.setSpeed(Gpio::SPEED_100MHz)
		.setAlternate(Gpio::OTG_FS_HS);

	USB_ID
		.setOpenDrain()
		.setFunction(Gpio::ALTERNATE)
		.setDirection(Gpio::OUTPUT)
		.setResistor(Gpio::PULL_UP)
		.setAlternate(Gpio::OTG_FS_HS);

	if(!USB_ID && USB_Vbus_det)
		log << "Vbus and USB OTG A found... dafuk." << Log::endl;
	else if(!USB_ID)
		log << "Found a USB OTG A, we're host" << Log::endl;
	else if(USB_Vbus_det)
		log << "Found vbus, we're guest" << Log::endl;
	else
		log << "Found neither VBus nor OTG A..." << Log::endl;

	USBD_Init(&USB_OTG_dev,
			USB_OTG_FS_CORE_ID,
			&USR_desc,
			&USBD_HID_cb,
			&USR_cb);

	UserButton
		.setDirection(Gpio::INPUT)
		.setResistor(Gpio::PULL_DOWN);


	Exti(UserButton)
		.enableRising()
		.enableIRQ()
		.setTopCB([] (int nr) { 
				hid_buf[1] += 10;
				USBD_HID_SendReport(&USB_OTG_dev,
					hid_buf,
					4);
		});
}
