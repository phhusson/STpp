#include <Led.h>
#include <Pwm.h>

extern Pwm LedG;
extern Pwm LedO;
extern Pwm LedB;
extern Pwm LedR;

extern Gpio UserButton;

//Drived by on-board logic...
//You'd better let it that way
extern Gpio USB_LedG;
extern Gpio USB_LedR;

extern Gpio USB_Vbus_en;
extern Gpio USB_Vbus_det;
extern Gpio USB_ID;
extern Gpio USB_DP;
extern Gpio USB_DM;
