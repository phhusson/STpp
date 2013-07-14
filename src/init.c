void clock_init(void) {
	//HSEON&~HSEBYP
	//Wait for HSERDY

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

	//PLLON
	//Wait for PLLRDY
	
	//Now let's use PLL's output as main clock
	//First setup dividors for AHB/APB1/APB2
	//Max AHB=168MHz, APB1=42MHz, APB2=84MHz
	//AHB divisor = 1, HPRE = 0
	//APB1 divisor = 4, PPRE1=b101=5
	//APB2 divisor = 2, PPRE2=b100=4

	//Then switch to apropriate clock source
	//SW=2
	//Wait for SWS=2
}
