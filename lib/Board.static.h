#include "Board.h"
#include "Led.h"
#include "Pwm.h"

/*
 *      O
 *  G        R
 *      G
 */
Pwm LedG(GpioD[12], Tim4, 1);
Pwm LedO(GpioD[13], Tim4, 2);
Pwm LedR(GpioD[14], Tim4, 3);
Pwm LedB(GpioD[15], Tim4, 4);

Gpio Lcd_RS(GpioA[1]);
Gpio Lcd_E(GpioA[2]);
Gpio Lcd_DB4(GpioB[11]);
Gpio Lcd_DB5(GpioB[12]);
Gpio Lcd_DB6(GpioB[13]);
Gpio Lcd_DB7(GpioB[14]);

//Rising edge
Gpio UserButton(GpioA[0]);

Gpio USB_Vbus_OC(GpioD[5]);
Gpio USB_Vbus_en(GpioC[0]);
Gpio USB_Vbus_det(GpioA[9]);
Gpio USB_ID(GpioA[10]);
Gpio USB_DM(GpioA[11]);
Gpio USB_DP(GpioA[12]);

Time time(Tim14);
