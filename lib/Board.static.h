#include "Board.h"
#include "Led.h"
#include "Pwm.h"
#include "HBridgeST.h"
#include "IncrementalEncoder.h"
#include "Uart.h"

// Motor control
Gpio Prop0_PWM(GpioB[10]);
Gpio Prop0A(GpioB[15]);
Gpio Prop0B(GpioB[13]);
HBridgeST HBridge0(Prop0A, Prop0B, Prop0_PWM, Tim2, 3);

Gpio Enc0A(GpioE[9]);
Gpio Enc0B(GpioE[11]);
IncrementalEncoder Encoder0(Enc0A, Enc0B, Tim1, 0, true);

Gpio Prop1_PWM(GpioB[11]);
Gpio Prop1A(GpioB[12]);
Gpio Prop1B(GpioB[14]);
HBridgeST HBridge1(Prop1A, Prop1B, Prop1_PWM, Tim2, 4);

Gpio Enc1A(GpioB[4]);
Gpio Enc1B(GpioB[5]);
IncrementalEncoder Encoder1(Enc1A, Enc1B, Tim3, 0, true);

Gpio Prop2_PWM(GpioC[7]);
Gpio Prop2A(GpioC[8]);
Gpio Prop2B(GpioC[12]);
HBridgeST Hbridge2(Prop2A, Prop2B, Prop2_PWM, Tim3, 2);

Gpio Prop3_PWM(GpioC[6]);
Gpio Prop3A(GpioC[9]);
Gpio Prop3B(GpioC[13]);
HBridgeST Hbridge3(Prop3A, Prop3B, Prop3_PWM, Tim3, 1);

//USB
Gpio USB_Vbus_OC(GpioD[5]);
Gpio USB_Vbus_en(GpioC[0]);
Gpio USB_Vbus_det(GpioA[9]);
Gpio USB_ID(GpioA[10]);
Gpio USB_DM(GpioA[11]);
Gpio USB_DP(GpioA[12]);

//Zigbee
Gpio Zigbee_RX(GpioC[11]);
Gpio Zigbee_TX(GpioC[10]);
//Can be either uart 3 or 4
//Need a Zigbee class
Uart Zigbee_UART(3);


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

//Other LEDs
//No pwm available for those
Gpio g_RLed1(GpioD[0]);
Gpio g_RLed2(GpioD[1]);
Gpio g_RLed3(GpioD[2]);
Gpio g_RLed4(GpioD[3]);

//Extra
Gpio UserButton(GpioA[0]);

//Which timer to use for precise (<1ms) timing
Time time(Tim14);
