#include <Board.h>
#include <tr1/functional>
#include <Log.h>
#include <Exti.h>
#include <Uart.h>
#include <Usb.h>
#include <Time.h>

int main() {
	Tim2
		.setAutoReload(1024)
		.setCaptureCompare1Sel(1)
		.setCaptureCompare2Sel(1)
		.setCaptureCompare1Polarity(1, 0)
		.setCaptureCompare2Polarity(1, 0)
		.setInputCapture1Filter(0)
		.setInputCapture2Filter(0)
		.setSlaveModeSelection(3)
		.enable();
/******************************************************************
 * Example de configurationd es registres fournit par la doc soft de la stm32f4xx
 * pour l'utilisation des compteurs incrementaux avec les timers 2-5.
 *
 * CC1S= ‘01’ (TIMx_CCMR1 register, TI1FP1 mapped on TI1)
 * -CC2S= ‘01’ (TIMx_CCMR2 register, TI2FP2 mapped on TI2)
 * -CC1P= ‘0’, CC1NP = ‘0’, IC1F =’0000’ (TIMx_CCER register, TI1FP1 noninverted, TI1FP1=TI1)
 * -CC2P= ‘0’, CC2NP = ‘0’, IC2F =’0000’ (TIMx_CCER register, TI2FP2 noninverted, TI2FP2=TI2)
 * -SMS= ‘011’ (TIMx_SMCR register, both inputs are active on both rising and falling edges) 
 * -CEN = 1 (TIMx_CR1 register, Counter is enabled) 
 *****************************************************************/

	while(1){
		time.msleep(200);
		log << "Counter alue : " << Tim4.getCounter() << endl;
	}
}
