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

Led LedG_USB(GpioA[9]);
Led LedR_USB(GpioD[5]);

//Rising edge
Gpio UserButton(GpioA[0]);
