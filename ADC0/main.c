#include<lpc17xx.h>

void uart_init(void);
void uart_send(char*);
void adc_init(void);
char adc_data_read();
char a[20];
char result_adc[10];

int main()
{
	char a[20]= {'H','e','l','r','s','t','y','j','o','p','k','s','\0'};
	int i;
	uart_init();
	adc_init();
	for(int i=0;i<20000;i++);
		uart_send(a);
	uart_send(a);
	while(1)
	{
	adc_data_read();
	uart_send(a);
	
	}
		


}

void uart_init()
{
	
	LPC_SC->PCONP |= (1<<3);
	LPC_SC->PCLKSEL0 |= (1<<6) | (0<<7);
	LPC_UART0->LCR |= (1<<7) | (1<<8);
	LPC_UART0->DLL = 36;
	LPC_UART0->DLM = 0;
	LPC_UART0->LCR |= (1<<0) | (1<<1) | (1<<2) | (1<<7);
	
	LPC_UART0->FDR = (2<<4) | 1;    
	LPC_UART0->FCR = (1<<0) | (1<<1) | (1<<2);
	
	LPC_PINCON->PINSEL0 = (1<<4) | (1<<6);  //pincon
	LPC_UART0->LCR &= ~(1<<7);
	
}
void uart_send(char* str)
{
	int i=0;
	while(str[i]!= '\0')
	{
	while(!(LPC_UART0->LSR & (1<<5)));
		LPC_UART0->THR = str[i];	
			i++;
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
	int i=0;
	while(!(LPC_ADC->ADDR0 & (1<<31)));
	register_data= LPC_ADC->ADDR0 & 0x0000fff0 ;
	register_data= (register_data>>4);
	
	while(0<=register_data)
		{
	     
		result_adc[i]=register_data%10;
		register_data=register_data/10;
		i++;
		
	}
	result_adc[i]='\0';
	
	//return result_adc ;	
	
}

	