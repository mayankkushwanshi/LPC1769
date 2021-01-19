#include <lpc214x.h>
#include <stdio.h>


void uart_init(void)
{
	PINSEL0|=(1<<16)|(1<<18);//enabling TXD and RXD
	VPBDIV=0x02;
	U1IER|=(1<<0);//Enabling Receive Data Enable Interrupt : Ref 11.3.6
	U1LCR |=(1<<7);
	U1DLL = 0x000000C3;
	U1LCR&=~(1<<7);
	U1LCR|=(1<<1)|(1<<0);// Setting 8 bit data and 1 stop bit and Enabling DLAB for configuring Baud rate Reg.

}

void tx_data(unsigned char data)
{
	while(!(U1LSR&(1<<5)));//Verifying Transmitter Holding Register Empty
	U1THR=data;
}

void uart_int() __irq
{
	unsigned char ch=U1RBR;// receiving data 
	unsigned long iir=U1IIR;//Sending interrupt identification information just for viewing "Don't take it personaly"
	//AD0INTEN|=(0<<3);
	tx_data((char)iir);//sendig this info to tx function
	//AD0INTEN|=(1<<3);
	VICVectAddr=0x00;//returning to main
	
}

void adc_int() __irq
{
	unsigned int data=AD0DR3;//Extracting Data from ADC Channel 3
	//AD0INTEN|=(0<<3);//(optional to disable channel 3 interrupt)
	if(data&(1<<31))// Taking data only if conversion is done which is tested by bit 31 which is DONE bit
	{
		unsigned int i, j;
		unsigned char ch[4]={'0','0','0','0'};
		data=(data>>6)&(0x3FF);//Extracting Result from bit 6 to 15
		sprintf(ch, "%d", data);//formatting data
		for(i=0;i<4;i++)
		{
			tx_data(ch[i]);
		}
		for(i=0;i<1;i++)//some delay
			for(j=0;j<35;j++);
	}
	//AD0INTEN|=(1<<3);//(if disable need to enable channel 3 interrupt)
	VICVectAddr=0x00;
}

void adc_init()
{
	PINSEL1|=(1<<28);// Selecting ADC ch 3 
	AD0CR=0x00;//reset
	AD0CR|=(1<<3)|/*(1<<13)|(1<<12)|(1<<11)|(1<<10)|(1<<9)|*/(1<<16)|(1<<21);//selecting channel (3), enabling burst for contineous //conversion,enabling adc 
	AD0CR|=(0x0000FF00);//providing this value for smallest clock less than 4.5Mhz
	AD0INTEN|=(1<<3);//enable channel 3 interrupt
}

void int_init()
{
	VICIntEnable|=(1<<18);//enabling AD0 interrupt
	VICVectCntl14=0x20|18;//enabling slot 14 and setting it to AD0
	VICVectAddr14=(unsigned long)adc_int;//assigning address of adc interrupt isr

	VICIntEnable|=(1<<7);//enabling uart1 int
	VICVectCntl0=0x20|7;////enabling slot 0 and setting it to UART1
	VICVectAddr0=(unsigned long)uart_int;//assigning address of uart 1 isr
}

int main()
{
	uart_init();
	int_init();
	adc_init();
	
	while(1);
}