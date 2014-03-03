#include "Board.h"
#include "Led.h"
#include "Pwm.h"
#include "HBridgeST.h"
#include "IncrementalEncoder.h"

// Motor control
Gpio Prop0_PWM(GpioB[10]);
Gpio Prop0A(GpioB[13]);
Gpio Prop0B(GpioB[15]);
HBridgeST HBridge0(Prop0A, Prop0B, Prop0_PWM, Tim2, 3);

Gpio Enc0A(GpioE[9]);
Gpio Enc0B(GpioE[11]);
IncrementalEncoder Encoder0(Enc0A, Enc0B, Tim1, 3, true);

Gpio Prop1_PWM(GpioB[11]);
Gpio Prop1A(GpioB[12]);
Gpio Prop1B(GpioB[14]);
HBridgeST HBridge1(Prop1A, Prop1B, Prop1_PWM, Tim2, 4);

//USB
Gpio USB_Vbus_OC(GpioD[5]);
Gpio USB_Vbus_en(GpioC[0]);
Gpio USB_Vbus_det(GpioA[9]);
Gpio USB_ID(GpioA[10]);
Gpio USB_DM(GpioA[11]);
Gpio USB_DP(GpioA[12]);


//On-board leds
/*
 *      O
 *  G        R
 *      G
 */
Gpio g_LedG(GpioD[12]);
Pwm LedG(g_LedG, Tim4, 1);

Gpio g_LedO(GpioD[13]);
Pwm LedO(g_LedO, Tim4, 2);

Gpio g_LedR(GpioD[14]);
Pwm LedR(g_LedR, Tim4, 3);

Gpio g_LedB(GpioD[15]);
Pwm LedB(g_LedB, Tim4, 4);

//Extra
Gpio UserButton(GpioA[0]);

//Which timer to use for precise (<1ms) timing
Time time(Tim14);
