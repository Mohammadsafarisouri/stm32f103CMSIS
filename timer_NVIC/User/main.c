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

void Timer2_Init(void);
int main(void) {
	uint32_t i = 0;
	
	Timer2_Init();
	LEDInit();
	USART2Init();
	
	
	

	
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
	USART2->DR = '*';
	while(((USART2->SR)&(USART_SR_TC))==0);
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

void Timer2_Init(void) {
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	TIM2->CR1 |= TIM_CR1_ARPE;									//auto reload preload enable 
	TIM2->CR1 &= (~TIM_CR1_CMS);								//(center aligned mode selection) detemain the upcounter or down counter(by direction)or aligned mode
	TIM2->CR1 &= (~TIM_CR1_DIR);								// Directin 0 up 1 down
	TIM2->CR1 &= (~TIM_CR1_URS);								// update request source 
	TIM2->CR1 &= (~TIM_CR1_UDIS);								// Update disable:1--->The Update event is not generated, shadow registers keep their value

	
	TIM2->CR2 &= (~TIM_CR2_MMS);								// master mode selection: 00-->internal clock

	TIM2->DIER |= TIM_DIER_UIE;									// Update interrupt enabled
	
	TIM2->PSC = 36000-1;
	TIM2->ARR = 4000;
	
	NVIC_EnableIRQ(TIM2_IRQn);
	TIM2->CR1 |= TIM_CR1_CEN;										//enable
	
}


