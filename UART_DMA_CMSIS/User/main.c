#include "stm32f10x.h"                  // Device header

int counter = 0;
char data[30] = "Ali Safari Souri\n";
void UART2InitDMA(void);
void sendStringUART2(char * data_str);
void delayMiliSecond(int ms);
void SysTick_Handler(void) {
	counter++;
	if(counter==3000) counter=0;
}

int main(void) {
	RCC->APB2ENR |= RCC_APB2ENR_IOPEEN;	
	GPIOE->CRL |= (GPIO_CRL_MODE5|GPIO_CRL_CNF5_0);
	/**************************************************************************/
	UART2InitDMA();
	SysTick_Config(72000);
	
	while(1) {
		delayMiliSecond(500);
		//send_str_USART2(data);
		GPIOE->ODR =(1<<5);
		delayMiliSecond(500);
		GPIOE->ODR &= (~(1<<5));
	}
}

void sendStringUART2(char * data_str) {
	USART2->DR = '*';
	while(((USART2->SR)&(USART_SR_TC))==0);
	while(*data_str != '\0') {
		USART2->DR = *data_str;
		while(((USART2->SR)&(USART_SR_TC))==0);
		data_str++;
	}
}

void UART2InitDMA(void) {
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	RCC->AHBENR |= RCC_AHBENR_DMA1EN;
	
	/* USART Configuration */
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
	/**************************************************************************/

	/* DMA Configuration */
	DMA1_Channel7->CCR &= (~DMA_CCR7_EN);							// Disable the DMA
	
	DMA1->IFCR = (DMA_IFCR_CHTIF7|DMA_IFCR_CGIF7|DMA_IFCR_CTCIF7|DMA_IFCR_CHTIF7);
	
	DMA1_Channel7->CPAR = (uint32_t)&USART2->DR;			// Peripheral Base Address
	DMA1_Channel7->CMAR = (uint32_t)data;							// Memory Base Address
	DMA1_Channel7->CNDTR = (uint32_t)17;												// Nubmer of Data to transfer
	DMA1_Channel7->CCR |= DMA_CCR7_PL_0;							// Channel Periority
	DMA1_Channel7->CCR |= DMA_CCR7_DIR;								// Memory to Peripheral mode
	DMA1_Channel7->CCR &= (~DMA_CCR7_CIRC);						// Circular mode OFF
	DMA1_Channel7->CCR |= DMA_CCR7_MINC;							// Memeory Incriment ON
	DMA1_Channel7->CCR &= (~DMA_CCR7_PINC);						// Periphral Incriment OFF
	DMA1_Channel7->CCR &= (~DMA_CCR7_MSIZE);					// Memeory size 8 bit
	DMA1_Channel7->CCR &=(~DMA_CCR7_PSIZE);						// Periphral size 8 bit
	DMA1_Channel7->CCR &= (~DMA_CCR7_MEM2MEM);				// Memory to Memory mode is OFF
	DMA1_Channel7->CCR |= DMA_CCR7_EN;								// DMA Enable 
	
	
	USART2->CR3 |= USART_CR3_DMAT;
}


void delayMiliSecond(int ms) {
	while(counter<=ms);
	if(counter>=ms) counter=0;
}
