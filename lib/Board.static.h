#include "Led.h"
#include "Pwm.h"

Led LedG(GpioD[12]);
//Led LedO(GpioD[13]);
Pwm  LedO(GpioD[13], Tim4, 2);
