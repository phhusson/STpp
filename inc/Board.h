#include <Led.h>
#include <Pwm.h>
#include <Time.h>
#include <HBridgeST.h>

extern Pwm LedG;
extern Pwm LedO;
extern Pwm LedB;
extern Pwm LedR;

extern Gpio UserButton;

extern Gpio Lcd_RS;
extern Gpio Lcd_E;
extern Gpio Lcd_DB4;
extern Gpio Lcd_DB5;
extern Gpio Lcd_DB6;
extern Gpio Lcd_DB7;

// Motor
extern Gpio Prop0_PWM;
extern Gpio Prop0A;
extern Gpio Prop0B;
extern HBridgeST HBridge0;

extern Gpio Prop1_PWM;
extern Gpio Prop1A;
extern Gpio Prop1B;
extern HBridgeST HBridge1;

// Rotary encoder
extern Gpio RotaryA;
extern Gpio RotaryB;

//Drived by on-board logic...
//You'd better let it that way
extern Gpio USB_LedG;
extern Gpio USB_LedR;

extern Gpio USB_Vbus_en;
extern Gpio USB_Vbus_det;
extern Gpio USB_ID;
extern Gpio USB_DP;
extern Gpio USB_DM;

extern Time time;
