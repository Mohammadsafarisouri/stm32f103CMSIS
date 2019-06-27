#include "stm32f10x.h"                  // Device header



int counterMiliSecond = 0;
char charCounter = 'A';
char data_send_ms = 'a';
uint8_t flagSlave = 0;
uint8_t flagMaster = 0;
char bufferSlave[28];
char bufferMaster[28];
char counterMaster[28] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ+\0";
char counter_Slave[28] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ-\0";

void SPI1Init(void);
void SPI2IInit(void);
void USART2Init(void);
void DMAInit(void);
	
void sendStringUSART2(char * data);
void delayMiliSecond(int ms);
void SysTick_Handler(void) {
	counterMiliSecond++;
	if(counterMiliSecond == 3000) counterMiliSecond = 0;
}





int main(void) {
	RCC->APB2ENR |= RCC_APB2ENR_IOPEEN;
	GPIOE->CRL |= (GPIO_CRL_MODE5|GPIO_CRL_CNF5_0);
	SysTick_Config(72000);
	SPI1Init();
	SPI2IInit();
	DMAInit();
	USART2Init();
	while(1) {
		sendStringUSART2("1- ");
		sendStringUSART2(bufferSlave);
		
		GPIOE->ODR =(1<<5);
		delayMiliSecond(500);
		sendStringUSART2("2- ");
		sendStringUSART2(bufferMaster);
		GPIOE->ODR &= (~(1<<5));
		delayMiliSecond(500);
	}
}

void sendStringUSART2(char * data) {
	while(*data != '\0') {
		USART2->DR = *data;
		while(((USART2->SR)&(USART_SR_TC))==0);
		data++;
	}
}


void delayMiliSecond(int ms) {
	while(counterMiliSecond <= ms);
	if(counterMiliSecond >= ms) counterMiliSecond=0;
}
void SPI1Init(void) {
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;			//SPI1 & USART2
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
	/*SPI1 as Master*/
	//A4----->SPI1_NSS
	//A5----->SPI1_SCK
	//A6----->SPI1_MISO
	//A7----->SPI1_MOSI
	
	//SPI1 GPIO Pins
	
	GPIOA->CRL |= GPIO_CRL_MODE4_0;
	GPIOA->CRL &=(~GPIO_CRL_CNF4_0);
	GPIOA->CRL |= GPIO_CRL_CNF4_1;
	
	GPIOA->CRL |= GPIO_CRL_MODE5_0;
	GPIOA->CRL &=(~GPIO_CRL_CNF5_0);
	GPIOA->CRL |= GPIO_CRL_CNF5_1;
	
	GPIOA->CRL &= (~GPIO_CRL_MODE6);
	
	GPIOA->CRL |= GPIO_CRL_MODE7_0;
	GPIOA->CRL &=(~GPIO_CRL_CNF7_0);
	GPIOA->CRL |= GPIO_CRL_CNF7_1;
	
	/*--------SPI1 Configuration---------*/
	
	SPI1->CR1 |= (SPI_CR1_BR_2|SPI_CR1_BR_0);
	SPI1->CR1 &= (~SPI_CR1_CPHA);
	SPI1->CR1 &= (~SPI_CR1_CPOL);
	SPI1->CR1 &= (~SPI_CR1_DFF);
	SPI1->CR1 &= (~SPI_CR1_LSBFIRST);
	SPI1->CR2 |= SPI_CR2_SSOE;
	SPI1->CR1 |= SPI_CR1_MSTR;
	
	SPI1->CR1 |= SPI_CR1_SPE;
	
	

	
}
void SPI2IInit(void) {
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;			//SPI2
	RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;
	/*SPI2 as Slave*/
	//B12----->SPI2_NSS
	//B13----->SPI2_SCK
	//B14----->SPI2_MISO
	//B15----->SPI2_MOSI
	
	//SPI2 GPIO Pins
	GPIOB->CRH &= (~GPIO_CRH_MODE12);
	
	GPIOB->CRH &= (~GPIO_CRH_MODE13);
	
	GPIOB->CRH |= GPIO_CRH_MODE14_0;
	GPIOB->CRH &= (~GPIO_CRH_CNF14_0);
	GPIOB->CRH |= GPIO_CRH_CNF14_1;
	
	GPIOB->CRH &= (~GPIO_CRH_MODE15);
	
/*--------SPI2 Configuration---------*/
	SPI2->CR1 |= SPI_CR1_BR_2;
	SPI2->CR1 &= (~SPI_CR1_CPHA);
	SPI2->CR1 &= (~SPI_CR1_CPOL);
	SPI2->CR1 &= (~SPI_CR1_MSTR);
	SPI2->CR1 &= (~SPI_CR1_DFF);
	
	SPI2->CR1 |= SPI_CR1_SPE;
}
void USART2Init(void) {
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	
	/*USART2*/
	//A2----->USART2_TX
	//A3----->USART2_RX
	GPIOA->CRL |= GPIO_CRL_MODE2_0;
	GPIOA->CRL &= (~GPIO_CRL_CNF2_0);
	GPIOA->CRL |= GPIO_CRL_CNF2_1;
	
	GPIOA->CRL &= (~GPIO_CRL_MODE3);
	
	/*--------USART2 Configuration---------*/
	/*Tx Configuration*/
	USART2->CR1 |= USART_CR1_UE;
	USART2->CR1 &= (~USART_CR1_M);
	USART2->CR2 &= (~USART_CR2_STOP);
	USART2->BRR |= 0x0753;
	USART2->CR1 |= USART_CR1_TE;
	
	/*Rx Configuration*/
	USART2->CR1 |= USART_CR1_RE;
	
}
void DMAInit(void) {
	RCC->AHBENR |= RCC_AHBENR_DMA1EN;
	/* DMA Configuration */
	/*--------DMA1 SPI1-------*/
	/* SPI1 Rx*/
	DMA1_Channel2->CPAR = (uint32_t)&SPI1->DR;
	DMA1_Channel2->CMAR = (uint32_t)bufferMaster;
	DMA1_Channel2->CNDTR = 28;
	DMA1_Channel2->CCR |= DMA_CCR2_PL_1;
	DMA1_Channel2->CCR &= (~DMA_CCR2_DIR);
	DMA1_Channel2->CCR |= DMA_CCR2_CIRC;
	DMA1_Channel2->CCR |= DMA_CCR2_MINC;
	DMA1_Channel2->CCR &= (~DMA_CCR2_PINC);
	DMA1_Channel2->CCR &= (~DMA_CCR2_MSIZE);
	DMA1_Channel2->CCR &= (~DMA_CCR2_PSIZE);
	DMA1_Channel2->CCR &= (~DMA_CCR2_HTIE);
	DMA1_Channel2->CCR &= (~DMA_CCR2_TEIE);
	DMA1_Channel2->CCR &= (~DMA_CCR2_MEM2MEM);
	DMA1_Channel2->CCR |= DMA_CCR2_EN;
	
	/* SPI1 Tx*/
	DMA1_Channel3->CPAR = (uint32_t)&SPI1->DR;
	DMA1_Channel3->CMAR = (uint32_t)counterMaster;
	DMA1_Channel3->CNDTR = 28;
	DMA1_Channel3->CCR |= DMA_CCR3_PL_0;
	DMA1_Channel3->CCR |= DMA_CCR3_DIR;
	DMA1_Channel3->CCR |= DMA_CCR3_CIRC;
	DMA1_Channel3->CCR |= DMA_CCR3_MINC;
	DMA1_Channel3->CCR &= (~DMA_CCR3_PINC);
	DMA1_Channel3->CCR &= (~DMA_CCR3_MSIZE);
	DMA1_Channel3->CCR &= (~DMA_CCR3_PSIZE);
	DMA1_Channel3->CCR &= (~DMA_CCR3_HTIE);
	DMA1_Channel3->CCR &= (~DMA_CCR3_TEIE);
	DMA1_Channel3->CCR &= (~DMA_CCR3_MEM2MEM);
	DMA1_Channel3->CCR |= DMA_CCR3_EN;
	
	
	
	/*--------DMA1 SPI2-------*/
	/* SPI2 Rx*/
	DMA1_Channel4->CPAR = (uint32_t)&SPI2->DR;
	DMA1_Channel4->CMAR = (uint32_t)bufferSlave;
	DMA1_Channel4->CNDTR = 28;
	DMA1_Channel4->CCR |= DMA_CCR4_PL_0;
	DMA1_Channel4->CCR &= (~DMA_CCR4_DIR);
	DMA1_Channel4->CCR |= DMA_CCR4_CIRC;
	DMA1_Channel4->CCR |= DMA_CCR4_MINC;
	DMA1_Channel4->CCR &= (~DMA_CCR4_PINC);
	DMA1_Channel4->CCR &= (~DMA_CCR4_MSIZE);
	DMA1_Channel4->CCR &= (~DMA_CCR4_PSIZE);
	DMA1_Channel4->CCR &= (~DMA_CCR4_HTIE);
	DMA1_Channel4->CCR &= (~DMA_CCR4_TEIE);
	DMA1_Channel4->CCR &= (~DMA_CCR4_MEM2MEM);
	DMA1_Channel4->CCR |= DMA_CCR4_EN;
	/* SPI2 Tx*/
	DMA1_Channel5->CPAR = (uint32_t)&SPI2->DR;
	DMA1_Channel5->CMAR = (uint32_t)counter_Slave;
	DMA1_Channel5->CNDTR = 28;
	DMA1_Channel5->CCR |= DMA_CCR5_PL_0;
	DMA1_Channel5->CCR |= DMA_CCR5_DIR;
	DMA1_Channel5->CCR |= DMA_CCR5_CIRC;
	DMA1_Channel5->CCR |= DMA_CCR5_MINC;
	DMA1_Channel5->CCR &= (~DMA_CCR5_PINC);
	DMA1_Channel5->CCR &= (~DMA_CCR5_MSIZE);
	DMA1_Channel5->CCR &= (~DMA_CCR5_PSIZE);
	DMA1_Channel5->CCR &= (~DMA_CCR5_HTIE);
	DMA1_Channel5->CCR &= (~DMA_CCR5_TEIE);
	DMA1_Channel5->CCR &= (~DMA_CCR5_MEM2MEM);
	DMA1_Channel5->CCR |= DMA_CCR5_EN;
	
	
	SPI1->CR2 |= SPI_CR2_TXDMAEN;								//Very Important to be hear(After confguration SPI & DMA, the DMA request most be send)
	SPI1->CR2 |= SPI_CR2_RXDMAEN;
	SPI2->CR2 |= SPI_CR2_TXDMAEN;
	SPI2->CR2 |= SPI_CR2_RXDMAEN;
	
}
