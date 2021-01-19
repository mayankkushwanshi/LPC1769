#include <LPC17xx.h>
#include <stdio.h>
void Uart3_Transmit(unsigned char ch);

void initUart3(void)
{
	LPC_SC->PCONP=(1<<25);
	LPC_SC->PCLKSEL1=(1 << 19);	
	LPC_UART3->IER|=(1<<0);
	LPC_PINCON->PINSEL0|=(1 << 1)|(1<< 3);
	LPC_UART3->LCR|=(1<<7);
	LPC_UART3->DLL = 235;
	LPC_UART3->LCR&=~(1<<7);
	LPC_UART3->LCR = 0x03;
}

void Uart3_Transmit(unsigned char ch)
{
	while(!(LPC_UART3->LSR&(1<<5)));//Verifying Transmitter Holding Register Empty
	LPC_UART3->THR=ch;	
}

void UART3_IRQHandler(void)
{
	unsigned char ch=LPC_UART3->RBR;// receiving data 
	unsigned long iir=LPC_UART3->IIR;//Sending interrupt identification information just for viewing "Don't take it personaly"
	//LPC_ADC0->ADINTEN|=(0<<2);
	Uart3_Transmit((char)iir);//sendig this info to tx function
	//LPC_ADC0->ADINTEN|=(1<<2);
}

void ADC_IRQHandler(void)
{
	unsigned int data=LPC_ADC->ADDR2;//Extracting Data from ADC Channel 2
	//LPC_ADC->ADINTEN|=(0<<2);//(optional to disable channel 2 interrupt)
	if(data&(1<<31))// Taking data only if conversion is done which is tested by bit 31 which is DONE bit
	{
		unsigned int i, j;
		unsigned char ch[4]={'0','0','0','0'};
		data=(data>>4)&(0xFFF);//Extracting Result from bit 4 to 15
		sprintf(ch, "%d", data);//formatting data
		for(i=0;i<4;i++)
		{
			Uart3_Transmit(ch[i]);
		}
		Uart3_Transmit(0x09);
		for(i=0;i<1;i++)//some delay
			for(j=0;j<35;j++);
	}
	//LPC_ADC->ADINTEN|=(1<<2);//(if disable need to enable channel 2 interrupt)	
}

void adc_init()
{
	LPC_SC->PCONP|=(1<<12);
	LPC_SC->PCLKSEL1|=(1 << 24);
	LPC_PINCON->PINSEL1|=(1 << 18);
	LPC_ADC->ADCR=0x01;//reset
	LPC_ADC->ADCR|=(1<<2)|(1<<16)|(1<<21);//selecting channel (2), enabling burst for contineous //conversion,enabling adc 
	LPC_ADC->ADCR|=(0x0000FF00);//providing this value for smallest clock less than 4.5Mhz
	LPC_ADC->ADINTEN|=(1<<2);//enable channel 2 interrupt
}

int main()
{
	SystemInit();	
	initUart3();
	adc_init();
	NVIC_EnableIRQ(ADC_IRQn); //Call API to enable interrupt in NVIC
    NVIC_SetPriority(ADC_IRQn,255); //priority 
	NVIC_EnableIRQ(UART3_IRQn);
    NVIC_SetPriority(UART3_IRQn,0); //priority	
	while(1);
}
