#include "Board.h"
#include "Led.h"
#include "Pwm.h"
#include "HBridgeST.h"
#include "IncrementalEncoder.h"
#include "Uart.h"
#include "Ax12.h"

// Motor control
Gpio Prop0_PWM(GpioB[10]);
Gpio Prop0A(GpioB[13]);
Gpio Prop0B(GpioB[15]);
HBridgeST HBridge0(Prop0A, Prop0B, Prop0_PWM, Tim2, 3);

Gpio Enc0A(GpioE[9]);
Gpio Enc0B(GpioE[11]);
IncrementalEncoder Encoder0(Enc0A, Enc0B, Tim1, 0, false);

Gpio Prop1_PWM(GpioB[11]);
Gpio Prop1A(GpioB[14]);
Gpio Prop1B(GpioB[12]);
HBridgeST HBridge1(Prop1B, Prop1A, Prop1_PWM, Tim2, 4);

Gpio Enc1A(GpioB[4]);
Gpio Enc1B(GpioB[5]);
IncrementalEncoder Encoder1(Enc1A, Enc1B, Tim3, 0, true);

Gpio Prop2_PWM(GpioC[7]);
Gpio Prop2A(GpioC[8]);
Gpio Prop2B(GpioC[12]);
HBridgeST Hbridge2(Prop2A, Prop2B, Prop2_PWM, Tim8, 2);

Gpio Prop3_PWM(GpioC[6]);
Gpio Prop3A(GpioC[9]);
Gpio Prop3B(GpioC[13]);
HBridgeST Hbridge3(Prop3A, Prop3B, Prop3_PWM, Tim8, 1);

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
//TODO: Need a Zigbee class
DmaStream ZigbeeUartDma(1, 4, 4);
Uart Zigbee_UART(4, &ZigbeeUartDma);

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

Gpio Ax12Tx(GpioA[2]);
Gpio Ax12Rx(GpioA[3]);
Gpio Ax12En(GpioA[1]);
DmaStream Ax12UartDma(1, 6, 4);
Uart Ax12_UART(2, &Ax12UartDma);

Ax12 mamoutorRight(Ax12_UART, 144, &Ax12En, Ax12Rx, Ax12Tx);

Ax12 bacLeftExternal(Ax12_UART, 141, &Ax12En, Ax12Rx, Ax12Tx);
Ax12 bacLeftCentered(Ax12_UART, 129, &Ax12En, Ax12Rx, Ax12Tx);
Ax12 bacLeftReservoir(Ax12_UART, 121, &Ax12En, Ax12Rx, Ax12Tx);

Ax12 bacRightCentered(Ax12_UART, 20, &Ax12En, Ax12Rx, Ax12Tx);
Ax12 bacRightReservoir(Ax12_UART, 142, &Ax12En, Ax12Rx, Ax12Tx);

BacAFruits bacLeft(&bacLeftExternal, &bacLeftCentered, &bacLeftReservoir, true);
BacAFruits bacRight(NULL, &bacRightCentered, &bacRightReservoir, false);

Ax12 ax12Broadcast(Ax12_UART, 0xfe, &Ax12En, Ax12Rx, Ax12Tx);

Gpio External_TX(GpioD[8]);
Gpio External_RX(GpioD[9]);
DmaStream ExternalUartDma(1, 3, 4);
Uart ExternalUart(3, &ExternalUartDma);
