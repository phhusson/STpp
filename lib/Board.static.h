#include "Board.h"
#include "Led.h"
#include "Pwm.h"
#include "HBridgeST.h"

// Motor control
Gpio Prop0_PWM(GpioB[10]);
Gpio Prop0A(GpioB[13]);
Gpio Prop0B(GpioB[15]);
HBridgeST HBridge0(Prop0A, Prop0B, Prop0_PWM, Tim2, 3);

Gpio Prop1_PWM(GpioB[11]);
Gpio Prop1A(GpioB[12]);
Gpio Prop1B(GpioB[14]);
HBridgeST HBridge1(Prop1A, Prop1B, Prop1_PWM, Tim2, 4);

//Rising edge
Gpio UserButton(GpioA[0]);

Gpio USB_Vbus_OC(GpioD[5]);
Gpio USB_Vbus_en(GpioC[0]);
Gpio USB_Vbus_det(GpioA[9]);
Gpio USB_ID(GpioA[10]);
Gpio USB_DM(GpioA[11]);
Gpio USB_DP(GpioA[12]);

Time time(Tim14);

Gpio g_LedG(GpioD[12]);
Gpio g_LedO(GpioD[13]);
Gpio g_LedR(GpioD[14]);
Gpio g_LedB(GpioD[15]);

/*
 *      O
 *  G        R
 *      G
 */
Pwm LedG(g_LedG, Tim4, 1);
Pwm LedO(g_LedO, Tim4, 2);
Pwm LedR(g_LedR, Tim4, 3);
Pwm LedB(g_LedB, Tim4, 4);
