#include <stm32f4xx.h>

void init(void) {
	//HSEON&~HSEBYP
	RCC->CR = (RCC->CR | RCC_CR_HSEON) & ~RCC_CR_HSEBYP;
	//Wait for HSERDY
	while( !(RCC->CR&RCC_CR_HSERDY) );

	//cf p125 for clocks derivation
	//We want main clock @168MHz, and USG OTG FS @48MHz
	// USB OTG FS = 2/7 * Main Clock(168MHz) = 48MHz 

	// PLLP = 2, PLLQ = 7, fVCO = 168*2=336MHz
	// Clock source = HSE
	// HSE = 24MHz crystal; 336=24*14
	// PLLN = 14, PLLM = 1
	// PLLM must be >=2
	// PLLN = 28, PLLM=2,

	// real registers values:
	// PLLQ=7, PLLSRC=1, PLLP=0, PLLN=28, PLLM=2
	RCC->PLLCFGR =
		/* PLLQ */ 7 << 24 |
		/* PLLSRC */ 1 << 22 |
		/* PLLP */ 0 << 16 |
		/* PLLN */ 28 << 6 |
		/* PLLM */ 2 << 0;

	//PLLON
	RCC->CR = RCC->CR | RCC_CR_PLLON;
	//Wait for PLLRDY
	while( !(RCC->CR & RCC_CR_PLLRDY) );
	
	//Now let's use PLL's output as main clock
	//First setup dividors for AHB/APB1/APB2
	//Max AHB=168MHz, APB1=42MHz, APB2=84MHz
	//AHB divisor = 1, HPRE = 0
	//APB1 divisor = 4, PPRE1=b101=5
	//APB2 divisor = 2, PPRE2=b100=4
	RCC->CFGR = ( RCC->CFGR & ~RCC_CFGR_HPRE & ~RCC_CFGR_PPRE1 & ~RCC_CFGR_PPRE2 ) |
		/* HPRE */ 0 << 4 |
		/* PPRE1 */ 5 << 10 |
		/* PPRE2 */ 4 << 13;

	//Then switch to apropriate clock source
	//SW=2
	RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_SW) | 2 << 0;
	//Wait for SWS=2
	while( (RCC->CFGR & RCC_CFGR_SWS) != (2<<2) );

	extern char _sbss,_ebss;
	char *b=&_sbss;
	while( b < _ebss)
		*b++=0;

	vTaskStartScheduler();

	extern void (**__init_array_start)();
	extern void (**__init_array_end)();
	unsigned long *ptr;
	void (*p)();
	for (ptr = &__init_array_start; ptr < &__init_array_end; ++ptr) {
		p = (*ptr);
		p();
	}
}
