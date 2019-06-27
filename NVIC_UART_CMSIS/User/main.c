#include "stm32f10x.h"                  // Device header


char data[30] = "Ali\n";
void sendStringUART2(char * data_str);
void UART2Init(void);

void USART2_IRQHandler(void) {
	
	static char counter =0;
	
	if(((USART2->SR)&(USART_SR_RXNE))!=0) {
		data[counter] = USART2->DR;
		counter++;
		USART2->SR &= (~USART_SR_RXNE);
		if(counter>4) {
			counter = 0;
			sendStringUART2(data);
		}
	}
}



int main(void) {
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
	RCC->APB2ENR |= RCC_APB2ENR_IOPEEN;
	
	GPIOE->CRL |= (GPIO_CRL_MODE5|GPIO_CRL_CNF5_0);
	UART2Init();	
	
	while(1) {
	}
}

void UART2Init(void) {
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
	
	
	
	NVIC_EnableIRQ(USART2_IRQn);

	//USART2->CR1 |=  USART_CR1_TXEIE;
	USART2->CR1 |= USART_CR1_RXNEIE;
}


void sendStringUART2(char * data_str) {
	uint32_t i = 0;
	USART2->DR = '*';
	while(((USART2->SR)&(USART_SR_TC))==0);
	while(*data_str != '\0') {
		USART2->DR = *data_str;
		while(((USART2->SR)&(USART_SR_TC))==0);
		data_str++;
	}
	for(i=0;i<=1800000;i++);
}


