#include "stm32f10x.h"                  // Device header
#include "adc.h"


#define V25  			 1.43					// min = 1.32 max = 1.52 typ = 1.43
#define Avg_slop	 0.0043				//min = 0.004 max = 0.0046 type = 0.0043		

extern uint16_t ADCBuffer[3];
	
int main(void)
{

	uint32_t i = 0;
	uint16_t adc_data = 0;
	float VTemp = 0;
	float Temp = 0;
	uint16_t adc_data2 = 0;
	char data[7] = "Ali    ";
	
	ADC1Init();
	
	
	
	
	
	while(1) {
		adc_data = ADCBuffer[0];

		for(i=0;i<720000;i++);
		
		data[0] =(char)((adc_data/10000))+0x30;
		data[1] = (char)((adc_data/1000)%10)+0x30;
		data[2] = (char)((adc_data/100)%10)+0x30;
		data[3] = (char)((adc_data/10)%10)+0x30;
		data[4] = (char)((adc_data)%10)+0x30;
		data[5] = '\0';

		adc_data2 = ADCBuffer[2];
		
		VTemp = (3.3*adc_data2)/4095.0;
		Temp = (V25-VTemp)/Avg_slop+25;
		
		data[0] = (char)(((int)Temp/100)%10)+0x30;
		data[1] = (char)(((int)Temp/10)%10)+0x30;
		data[2] = (char)(((int)Temp)%10)+0x30;
		data[3] = '\0';
		
		for(i=0;i<=7200000;i++);
	}
}






