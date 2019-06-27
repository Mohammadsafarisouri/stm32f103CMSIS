#include "stm32f10x.h"                  // Device header





void sendString2UART2(char * data_str);
void UART2Init(void);
int main(void) {
	uint32_t i = 0;
	char data[30] = "Ali Dadash\n";
	
	
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	
	GPIOC->CRH |= GPIO_CRH_MODE13;
	GPIOC->CRH |= GPIO_CRH_CNF13_0;
	GPIOC->CRL |= (GPIO_CRL_MODE5|GPIO_CRL_CNF1_0);
	
	UART2Init();
	
	while(1) {
		GPIOC->ODR =(1<<13);
		for(i=0;i<=900000;i++);
		GPIOC->ODR &= (~(1<<13));
		for(i=0;i<=900000;i++);
		sendString2UART2(data);
	}
}

void UART2Init(void) {
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	//Configuration of GPIO
	//A2----->USART2_TX
	GPIOA->CRL |= GPIO_CRL_MODE2;
	GPIOA->CRL |= GPIO_CRL_CNF2;
	//A3----->USART2_RX
	GPIOA->CRL &= (~GPIO_CRL_MODE3);
	GPIOA->CRL |= GPIO_CRL_CNF3_0;
	
	//Configuration of UART in 19200
	USART2->CR1 |= (1<<13);
	USART2->CR1 |= (0<<12);
	USART2->CR2 &= (~USART_CR2_STOP);
	USART2->BRR = 0x0753;//0x0EA6 for 9600 Boldrate;0x0753 for 19200 ;
	
	USART2->CR1 |= (USART_CR1_TE);
	USART2->CR1 |= (USART_CR1_RE);
}


void sendString2UART2(char * data_str) {
	while(*data_str != '\0') {
		USART2->DR = *data_str;
		while(((USART2->SR)&(USART_SR_TC))==0);
		data_str++;
	}
}


