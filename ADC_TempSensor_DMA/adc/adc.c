#include "adc.h"


uint16_t ADCBuffer[3];

void ADC1GPIOConfig(void)
{
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;					//ADC8---->B0    & ADC9---->B1
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;

	
	GPIOB->CRL &= (~GPIO_CRL_MODE0);
	GPIOB->CRL &= (~GPIO_CRL_CNF0_0);
	
	
	GPIOB->CRL &= (~GPIO_CRL_MODE1);
	GPIOB->CRL &= (~GPIO_CRL_CNF1_0);
	
	
	
}
void ADC1ModeConfig(void)
{
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;				// 236 / 1133   ADC Registers in Reference manual
	
	ADC1->CR1 |= ADC_CR1_SCAN;								// enable scan mode
	
	
	
	ADC1->CR2 &= (~ADC_CR2_ALIGN);						// aligned right
	ADC1->CR2 |= (ADC_CR2_CONT);								// Conitnous Mode
	
	
	
	
	
	
	ADC1->SMPR2 |= (ADC_SMPR2_SMP8_2);					// sampling rate 41.5  for Channel8
	ADC1->SMPR2 |= (ADC_SMPR2_SMP9_2);
	ADC1->SMPR1 |= (ADC_SMPR1_SMP16_2);
	
	ADC1->SQR1 |= (uint32_t)((3-1)<<20);								// 2 channel is used

	ADC1->SQR3 |= ADC_SQR3_SQ1_3;												// channel 8 in sq1
	ADC1->SQR3 |= (ADC_SQR3_SQ2_3|ADC_SQR3_SQ2_0);			// channel 9 in sq2
	ADC1->SQR3 |= (ADC_SQR3_SQ3_4);											// channel 16 in sq3 (TempSensor)
	
	
	
	
	ADC1->CR2 |= ADC_CR2_DMA;
	ADC1->CR2 |= ADC_CR2_EXTSEL;
	
	ADC1->CR2 |= ADC_CR2_TSVREFE;

	ADC1->CR2 |= ADC_CR2_ADON;								// ON the ADC
	
	
	ADC1->CR2 |= ADC_CR2_RSTCAL;							// Reset Calibration
	while(((ADC1->CR2)&ADC_CR2_RSTCAL)!=0);
	ADC1->CR2 |= ADC_CR2_CAL;									// Set Calibration
	while(((ADC1->CR2)&ADC_CR2_CAL)!=0);
	
	
	ADC1->CR2 |= ADC_CR2_EXTTRIG;
	ADC1->CR2 |= ADC_CR2_SWSTART;
	
		
}
void DMA1ModeConfig(void)
{
	RCC->AHBENR |= RCC_AHBENR_DMA1EN;		//283 / 1133 DMA Registers in Reference manual
	
	DMA1_Channel1->CCR &= (DMA_CCR1_EN);		//Didable DMA1 Channel 1
	
	DMA1->IFCR = (DMA_IFCR_CHTIF1|DMA_IFCR_CGIF1|DMA_IFCR_CTCIF1|DMA_IFCR_CHTIF1);
	
	DMA1_Channel1->CPAR |= (uint32_t)&ADC1->DR;
	DMA1_Channel1->CMAR |= (uint32_t)ADCBuffer;
	
	DMA1_Channel1->CNDTR |= (uint32_t)3;
	
	DMA1_Channel1->CCR &= (~DMA_CCR1_MEM2MEM);
	DMA1_Channel1->CCR |= DMA_CCR1_PL_0;
	DMA1_Channel1->CCR |= DMA_CCR1_MSIZE_0;
	DMA1_Channel1->CCR |= DMA_CCR1_PSIZE_0;
	DMA1_Channel1->CCR |= DMA_CCR1_MINC;
	DMA1_Channel1->CCR &= (~DMA_CCR1_PINC);
	DMA1_Channel1->CCR |= DMA_CCR1_CIRC;
	DMA1_Channel1->CCR &= (~DMA_CCR1_DIR);
	DMA1_Channel1->CCR |= DMA_CCR1_EN;
	
	
	
	
	
}

void ADC1Init(void)
{
	ADC1GPIOConfig();
	DMA1ModeConfig();
	ADC1ModeConfig();

}
