#include "stm32f10x.h"                  // Device header

void sendStringUSART2(char * data_str);
void LEDInit(void);
void USART2Init(void);
char data[30] = "Ali Safari Souri\n";

void TIM2_IRQHandler(void) {
	if((TIM2->SR)&(TIM_SR_UIF)) {
		TIM2->SR &= (~TIM_SR_UIF);
		sendStringUSART2(data);
	}
}
void Timer2PWM(void);
int main(void) {

	uint32_t i = 0;
	
	LEDInit();
	USART2Init();
	Timer2PWM();
	
	while(1) {
	
		GPIOE->BSRR |= GPIO_BSRR_BS5;
		GPIOB->BSRR |= GPIO_BSRR_BS5;
		
		for(i=0;i<=2700000;i++);
		GPIOE->BSRR |= GPIO_BSRR_BR5;
		GPIOB->BSRR |= GPIO_BSRR_BR5;
		
		for(i=0;i<=2700000;i++);
	}
}

void sendStringUSART2(char * data_str) {
	
	while(*data_str != '\0') {
		USART2->DR = *data_str;
		while(((USART2->SR)&(USART_SR_TC))==0);
		//while((USART2->SR)&(USART_SR_TC)==0);
		data_str++;
	}
}

void USART2Init(void) {
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	
	//A2----->USART2_TX
	//A3----->USART2_RX
	GPIOA->CRL |= GPIO_CRL_MODE2;
	GPIOA->CRL |= GPIO_CRL_CNF2;
	
	
	GPIOA->CRL &= (~GPIO_CRL_MODE3);
	GPIOA->CRL |= GPIO_CRL_CNF3_0;

	
	USART2->CR1 |= (1<<13);
	USART2->CR1 |= (0<<12);
	USART2->CR2 &= (~USART_CR2_STOP);
	USART2->BRR = 0x0753;//0x0EA6 for 9600 Boldrate;0x0753 for 19200 ;
	
	USART2->CR1 |= (USART_CR1_TE);
	USART2->CR1 |= (USART_CR1_RE);
}

void LEDInit(void) {
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
	RCC->APB2ENR |= RCC_APB2ENR_IOPEEN;
	
	GPIOE->CRL |= (GPIO_CRL_MODE5|GPIO_CRL_CNF5_0);
	GPIOB->CRL |= (GPIO_CRL_MODE5|GPIO_CRL_CNF5_0);
}



void Timer2PWM(void) {
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	
	GPIOA->CRL |= GPIO_CRL_MODE0;
	GPIOA->CRL &= (~GPIO_CRL_CNF0_0);
	GPIOA->CRL |= GPIO_CRL_CNF0_1;
	
	GPIOA->CRL |= GPIO_CRL_MODE1;
	GPIOA->CRL &= (~GPIO_CRL_CNF1_0);
	GPIOA->CRL |= GPIO_CRL_CNF1_1;
	
	
	TIM2->CR1 |= TIM_CR1_ARPE;
	TIM2->CR1 &= (~TIM_CR1_CKD);
	TIM2->CR1 &= (~TIM_CR1_CMS);
	TIM2->CR1 &= (~TIM_CR1_DIR);
	TIM2->CR1 &= (~TIM_CR1_URS);
	TIM2->CR1 &= (~TIM_CR1_UDIS);
	
	TIM2->CR2 &= (~TIM_CR2_MMS);
	
	TIM2->DIER |= TIM_DIER_UIE;
	
	TIM2->PSC = 36000-1;
	TIM2->ARR = 4000;
	
	NVIC_EnableIRQ(TIM2_IRQn);
	
	//PWM Configuration   386 / 1133
	TIM2->CCR1 = 3000;		//		413 / 1133
	TIM2->CCR2 = 100;		//		413 / 1133
	TIM2->CCMR1 |= (TIM_CCMR1_OC1M_1|TIM_CCMR1_OC1M_2);					//pwm1 channel 1 -------> Pin A0
	TIM2->CCMR1 |= TIM_CCMR1_OC1PE;															// enable channel 1 preload register
	TIM2->CCMR1 |= (TIM_CCMR1_OC2M_1|TIM_CCMR1_OC2M_2);					// pwm1 channel 2--------> Pin A1
	TIM2->CCMR1 |= TIM_CCMR1_OC2PE;															// enable channel 2 preload register
	
	TIM2->CR1 |= TIM_CR1_ARPE;
	TIM2->EGR |= TIM_EGR_UG;																		// enble Update Event in Event Generetion Register(for update the shadow register)
	
	TIM2->CCER &= (~TIM_CCER_CC1P);															// determain active High or active Low (Polarity)
	TIM2->CCER &= (~TIM_CCER_CC2P);
	
	TIM2->CCER |= TIM_CCER_CC1E;																//  signal is output on the corresponding output pin
	TIM2->CCER |= TIM_CCER_CC2E;
	
	
	TIM2->CR1 |= TIM_CR1_CEN;
	
}
