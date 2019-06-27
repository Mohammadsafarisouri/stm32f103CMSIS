#include "stm32f10x.h"                  // Device header

uint16_t i = 0;
void LED_Init(void);
void SysTick_Handler(void) {
	i++;
	if(i>1000) {
		GPIOE->ODR |= (GPIO_ODR_ODR5|GPIO_ODR_ODR6);
	}
	else {
		GPIOE->ODR = 0;
	}
	if(i>=2000) i = 0;
}




int main(void) {
	SysTick_Config(72000);
	LED_Init();
	
	while(1) {
		
		
	}
}


// Configuration of LEDs Port E: Pin5, Pin6
void LED_Init(void) {
	RCC->APB2ENR |= RCC_APB2ENR_IOPEEN;
	GPIOE->CRL |= GPIO_CRL_MODE5;
	GPIOE->CRL &= ~GPIO_CRL_CNF5;
	GPIOE->ODR |= GPIO_ODR_ODR5;
	
	GPIOE->CRL |= GPIO_CRL_MODE6;
	GPIOE->CRL &= ~GPIO_CRL_CNF6;
	GPIOE->ODR |= GPIO_ODR_ODR6;
}


