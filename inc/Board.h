#include <Led.h>
#include <Pwm.h>
#include <Time.h>
#include <HBridgeST.h>
#include <IncrementalEncoder.h>


// Motor control
extern Gpio Prop0_PWM;
extern Gpio Prop0A;
extern Gpio Prop0B;
extern HBridgeST HBridge0;

extern Gpio Enc0A;
extern Gpio Enc0B;
extern IncrementalEncoder Encoder0;

extern Gpio Prop1_PWM;
extern Gpio Prop1A;
extern Gpio Prop1B;
extern HBridgeST HBridge1;

//USB
extern Gpio USB_Vbus_OC;
extern Gpio USB_Vbus_en;
extern Gpio USB_Vbus_det;
extern Gpio USB_ID;
extern Gpio USB_DM;
extern Gpio USB_DP;

//On-board leds
extern Pwm LedG;
extern Pwm LedO;
extern Pwm LedB;
extern Pwm LedR;

//Extra
extern Gpio UserButton;

//Which timer to use for precise (<1ms) timing
extern Time time;
