#include<lpc17xx.h>

//tx
void UART0_WriteChar(char ch)
{
    while(!((LPC_UART0->LSR)&(1<< 5)));  // The THRE Bit is 1 when THR is clear and is 0 when a write occurs on UnTHR
    LPC_UART0->THR = ch; 
}
//rx
char UART0_ReadChar()
{
    char ch; 
    while(!((LPC_UART0->LSR)&(1<<0)));  // 0-UART FIFO EMPTY, 1-UART FIFO FILLED
        ch = LPC_UART0->RBR;                // Read the received data    
    return ch;
}



//ADC initialization

#define ADC_CLOCK 1000000 

int main()
	
{
LPC_SC->PCONP |= (1<<3);
LPC_SC->PCLKSEL0 |= (0<<6) | (0<<7);
LPC_UART0->LCR = (0x03<<0)  | (1<<7);																	
LPC_UART0->DLL =  130;										                          
LPC_UART0->DLM =  0;														                      
LPC_UART0->FDR = (2<<4) | 1;
LPC_UART0->LCR |= (1<<0) | (1<<1) | (1<<2) | (1<<7);
LPC_UART0->FCR = (1<<0) | (1<<1) | (2<<1) ; 													
LPC_PINCON->PINSEL0 = (1<<4) | (1<<6);    
LPC_UART0->LCR &= ~(1<<7);  	
																																		
//ADC part
LPC_SC->PCONP |= (1 << 12); 
LPC_SC->PCLKSEL0 |= (1<<24) | (1 <<0) 	// full clock and watchdog time
	
	
LPC_PINCON->PINSEL1 &= ~0x002FC000; /* Funtions as ADC (0-3) pins when 01 is selected resp. under PINSEL1 reg.P0.23 (15,14), P0.24 (17,16), P0.25 (19,18), P0.26 (21,20)*/
LPC_PINCON->PINSEL1 |= (0x01 << 14); // We will be using AD0 channel only.
//clock = SystemFrequency/4;
LPC_ADC->ADCR |= (1 << 0); // Select channel 0 on ADC0
LPC_ADC->ADCR |= ((clock/ADC_CLOCK - 1) << 8); // CLKDIV = (clock/ADC_CLOCK - 1)
LPC_ADC->ADCR |= ((0 << 16) | (1 << 21) | (0 << 24) | (0 << 27));

LPC_ADC->ADCR &= 0xFFFFFF00;
LPC_ADC->ADCR |= (1 << 24) | (1 << 0); //Select AD0 channel and start the conversion process.


do{
DataReg = *(volatile unsigned long *)(LPC_ADC_BASE + (1<<4) + 4 * channelNum);
}while(!(DataReg & (1<<31))); // When ADC_DONE is 1, the conversion is complete.


LPC_ADC->ADCR &= 0xF8FFFFFF; //Stop AD0 conversion process.
LPC_ADC->ADDR0 = (DataReg>>4)&0xFFF; // The max value for the result will be in the range Vrefp (0xfff) and Vrefn (0x000)	
	
	



void split(unsigned char s)
{
unsigned char a[4];
int i=0;

while(i<4){
a[i] = s%10;
s = s/10;
i++;
}
do
{
--i;
Write_UART_Char(a[i]+48);
}while(i);
}



}

