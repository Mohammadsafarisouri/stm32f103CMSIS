#include "stm32f10x.h"                  // Device header



int counterMiliSecond = 0;
char charCounter = 'A';
char dataSendMiliSecond = 'a';
uint8_t flagSlave = 0;
uint8_t flagMaster = 0;
char dataSlave[29];
char dataMaster[29];
char data[30] = "Ali Safari Souri\n";


void sendStringUART2(char * data);
void delayMiliSecond(int ms);
void SysTick_Handler(void) {

	counterMiliSecond++;
	if(counterMiliSecond == 3000) counterMiliSecond = 0;
}
void SPI1_IRQHandler(void) {
	static char Master = 'A';
	static uint8_t i = 0;
	
	if((SPI1->SR)&(SPI_SR_TXE)) {
		SPI1->DR = Master;
		Master++;
		if(Master>0x5A) Master='A';
	}
	if(((SPI1->SR)&(SPI_SR_RXNE))) {
		dataMaster[i] = SPI1->DR;
		i++;
		if(i>25) {
			dataMaster[26] = '-';
			dataMaster[27] = '\n';
			dataMaster[28] = '\0';
			i = 0;
			flagMaster = 1;
		}
	}
}

void SPI2_IRQHandler(void) {
	static char Slave = 'A';
	static uint8_t i = 0;
	if(((SPI2->SR)&(SPI_SR_TXE))) {
		SPI2->DR = Slave;
		Slave++;
		if(Slave>0x5A) Slave='A';
	}
	if((SPI2->SR)&(SPI_SR_RXNE)) {
		dataSlave[i] = SPI2->DR;
		i++;
		if(i>25) {
			dataSlave[26] = '+';
			dataSlave[27] = '\n';
			dataSlave[28] = '\0';
			i = 0;
			flagSlave = 1;
		}
	}
}




void SPI1Master(void) {
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;			//SPI1 & USART2
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
	
	/*SPI1 as Master*/
	//A4----->SPI1_NSS
	//A5----->SPI1_SCK
	//A6----->SPI1_MISO
	//A7----->SPI1_MOSI
	
	//SPI1 GPIO Pins
	GPIOA->CRL |= GPIO_CRL_MODE4_0;
	GPIOA->CRL &= (~GPIO_CRL_CNF4_0);
	GPIOA->CRL |= GPIO_CRL_CNF4_1;
	
	GPIOA->CRL |= GPIO_CRL_MODE5_0;
	GPIOA->CRL &= (~GPIO_CRL_CNF5_0);
	GPIOA->CRL |= GPIO_CRL_CNF5_1;
	
	GPIOA->CRL &= (~GPIO_CRL_MODE6);
	GPIOA->CRL &= (~GPIO_CRL_CNF6_0);
	GPIOA->CRL |= GPIO_CRL_CNF6_0;
	
	GPIOA->CRL |= GPIO_CRL_MODE7_0;
	GPIOA->CRL &= (~GPIO_CRL_CNF7_0);
	GPIOA->CRL |= GPIO_CRL_CNF7_1;
	
	//GPIOA->BSRR |= GPIO_BSRR_BS4;
	
	
	//SPI1 Configuration
	SPI1->CR1 |= (SPI_CR1_BR_2|SPI_CR1_BR_0);			//Clock of APB2(72MHz)/64
	SPI1->CR1 &= (~SPI_CR1_CPHA);
	SPI1->CR1 &= (~SPI_CR1_CPOL);
	SPI1->CR1 &= (~SPI_CR1_DFF);
	SPI1->CR1 &= (~SPI_CR1_LSBFIRST);
	SPI1->CR2 |= SPI_CR2_SSOE;
	SPI1->CR2 |= SPI_CR2_TXEIE;
	SPI1->CR2 |= SPI_CR2_RXNEIE;
	SPI1->CR1 |= SPI_CR1_MSTR;
	SPI1->CR1 |= SPI_CR1_SPE;
	
}
void SPI2Slave(void) {
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;			//SPI2
	RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;
	/*SPI2 as Slave*/
	//B12----->SPI2_NSS
	//B13----->SPI2_SCK
	//B14----->SPI2_MISO
	//B15----->SPI2_MOSI
	
	//SPI2 GPIO Pins
	GPIOB->CRH &= (~GPIO_CRH_MODE12);
	GPIOB->CRH &= (~GPIO_CRH_CNF12_0);
	GPIOB->CRH |= GPIO_CRH_CNF12_0;
	
	GPIOB->CRH &= (~GPIO_CRH_MODE13);
	GPIOB->CRH &= (~GPIO_CRH_CNF13_0);
	GPIOB->CRH |= GPIO_CRH_CNF13_0;
	
	GPIOB->CRH |= GPIO_CRH_MODE14_0;
	GPIOB->CRH &= (~GPIO_CRH_CNF14_0);
	GPIOB->CRH |= GPIO_CRH_CNF14_1;
	
	GPIOB->CRH &= (~GPIO_CRH_MODE15);
	GPIOB->CRH &= (~GPIO_CRH_CNF15_0);
	GPIOB->CRH |= GPIO_CRH_CNF15_0;
	
	//SPI2 Configuration
	SPI2->CR1 |= (SPI_CR1_BR_2);			//Clock of APB2(36MHz)/32
	SPI2->CR1 &= (~SPI_CR1_CPHA);
	SPI2->CR1 &= (~SPI_CR1_CPOL);
	SPI2->CR1 &= (~SPI_CR1_DFF);
	SPI2->CR1 &= (~SPI_CR1_LSBFIRST);
	SPI2->CR1 &= (~SPI_CR1_MSTR);
	SPI2->CR2 |= SPI_CR2_TXEIE;
	SPI2->CR2 |= SPI_CR2_RXNEIE;
	SPI2->CR1 |= SPI_CR1_SPE;
}

void USART2Transmiter(void) {
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;			//SPI1 & USART2
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
	
	/*USART2*/
	//A2----->USART2_TX
	//A3----->USART2_RX
	
	GPIOA->CRL |= GPIO_CRL_MODE2;
	GPIOA->CRL &= (~GPIO_CRL_CNF2_0);
	GPIOA->CRL |= GPIO_CRL_CNF2;
	
	
	GPIOA->CRL &= (~GPIO_CRL_MODE3);
	GPIOA->CRL &= (~GPIO_CRL_CNF3_0);
	GPIOA->CRL |= GPIO_CRL_CNF3_0;
	
	//Configuration
	USART2->CR1 |= (1<<13);
	USART2->CR1 |= (0<<12);
	USART2->CR2 &= (~USART_CR2_STOP);
	USART2->BRR = 0x0753;//0x0EA6 for 9600 Boldrate;0x0753 for 19200 ;
	
	USART2->CR1 |= (USART_CR1_TE);
	USART2->CR1 |= (USART_CR1_RE);
}
	
int main(void) {
	RCC->APB2ENR |= RCC_APB2ENR_IOPEEN;
	
	GPIOE->CRL |= (GPIO_CRL_MODE5|GPIO_CRL_CNF5_0);
	USART2Transmiter();
	SPI1Master();
	SPI2Slave();
	
	NVIC_EnableIRQ(SPI1_IRQn);
	NVIC_EnableIRQ(SPI2_IRQn);
	SysTick_Config(72000);
	
	while(1) {
		if(flagSlave) {
			flagSlave = 0;
			sendStringUART2(dataSlave);
		}
		if(flagMaster) {
			flagMaster = 0;
			sendStringUART2(dataMaster);
		}
//		send_str_USART2(data);
		GPIOE->ODR =(1<<5);
		delayMiliSecond(500);
		GPIOE->ODR &= (~(1<<5));
		delayMiliSecond(500);
	}
}

void sendStringUART2(char * data) {
	USART2->DR = '*';
	while(((USART2->SR)&(USART_SR_TC)) == 0);
	while(*data != '\0') {
		USART2->DR = *data;
		while(((USART2->SR)&(USART_SR_TC)) == 0);
		data++;
	}
}


void delayMiliSecond(int ms) {
	while(counterMiliSecond <= ms);
	if(counterMiliSecond >= ms) counterMiliSecond = 0;
}

