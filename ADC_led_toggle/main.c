#include<lpc17xx.h>
void delay(void);
void adc_init(void);
char adc_data_read();
int main(void)
{
	LPC_GPIO0->FIODIR = (1<<22); // Configure pins 0 to 3 on Port 0 as Output
	//LPC_GPIO3->FIODIR = (1<<26);
	adc_init();
	while(1)
	{
		adc_data_read();
	}
}	

void delay(void) //Hardcoded delay function
{
	int count,i=0;
	for(count=0; count < 6000000; count++) // You can edit this as per your needs
	{
		i++; // something needs to be here else compiler will remove the for loop!
	}
}

void adc_init(void)
{
	
	LPC_SC->PCONP |= (1<<12);
	LPC_ADC->ADCR |= (1<<21) | (20<<8) ; 
	LPC_SC->PCLKSEL0 |= (1<<24) | (0<<25);
	LPC_PINCON->PINSEL1 |=(0<<14) | (0 <<15);	
	LPC_ADC->ADCR  |= (1<<24) | (0<<25) | (0 << 26)  |(1<<0);
	
}
char adc_data_read()
{
	long int register_data;
	int i=12;
	while(!(LPC_ADC->ADDR0 & (1<<31)));
	register_data= LPC_ADC->ADDR0 & 0x0000fff0 ;
	register_data= (register_data>>4);
	
	if(register_data > 2048)
	{
		LPC_GPIO0->FIOSET = (1<<22); // Output HIGH //RED
		delay();
		LPC_GPIO0->FIOCLR = (1<<22); // Output LOW		
	}
	/*if(register_data < 2048)
	{
		LPC_GPIO3->FIOSET = (1<<26); // Output HIGH	//BLUE
		delay();
		LPC_GPIO3->FIOCLR = (1<<26); // Output LOW
		//delay();	
	}*/


}

