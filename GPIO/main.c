#include "stm32f10x.h"                  // Device header


int main(void) {
	uint32_t i = 0;
	
	// Active Clock of Port C
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
	
	// GPIO_CRH_MODE13	---> Pin13 Output mode, max speed 50MHz
	// GPIO_CRH_CNF13_0	---> Pin13 General purpose output Open-drain
	GPIOC->CRH |= (GPIO_CRH_MODE13|GPIO_CRH_CNF13_0);
	

	
	while(1) {
		GPIOC->BSRR |= GPIO_BSRR_BR13;		//Bit 13 is Reset
		for(i = 0;i<7200000;i++);
		
		GPIOC->BSRR |= GPIO_BSRR_BS13;		//Bit 13 is Set
		for(i = 0;i<7200000;i++);
		
	}
}

