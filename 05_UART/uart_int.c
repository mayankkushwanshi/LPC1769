//polling
/*#include "LPC17xx.h"

void initUart3(void);
void Uart3_Transmit(unsigned char ch);
unsigned char Uart3_Receive(void);
unsigned char ch;

int main(void)
{
	SystemInit();
	initUart3(); //Initialize UART3
    while(1)
    {
		 ch=Uart3_Receive();
		 Uart3_Transmit(ch); 
    }
    	
}

void initUart3(void)
{
	LPC_SC->PCONP=(1<<25);
	LPC_SC->PCLKSEL1=(1 << 19);
	//LPC_SC->PCLKSEL0=(1 << 9);
	LPC_PINCON->PINSEL0=((1 << 1)|(1<< 3));
	//LPC_PINCON->PINSEL0=(1 << 30);
	//LPC_PINCON->PINSEL1=(1 << 0);
	LPC_UART3->LCR|=(1<<7);
	LPC_UART3->DLL = 235;
	LPC_UART3->LCR&=~(1<<7);
	LPC_UART3->LCR = 0x03;
}

unsigned char Uart3_Receive(void)
{
	while(!(LPC_UART3->LSR&(1<<0)));
	ch=LPC_UART3->RBR;
	return ch;
}

void Uart3_Transmit(unsigned char ch)
{
	while(!(LPC_UART3->LSR&(1<<5)));
	LPC_UART3->THR=ch;	
}*/

//interrupt
#include <LPC17xx.h>

void initUart3(void);
void Uart3_Transmit(unsigned char ch);
unsigned char Uart3_Receive(void);
void UART3_IRQHandler(void);

unsigned char ch;
uint32_t flag=0;

int main(void)
{
	SystemInit();
	initUart3(); //Initialize UART3
	NVIC_EnableIRQ(UART3_IRQn); 
    while(1)
    {
				if(flag==1)
				{
					Uart3_Transmit(ch);
					flag=0;
				} 
    }
}

void initUart3(void)
{
	LPC_SC->PCONP=(1<<25);
	LPC_SC->PCLKSEL1=(1 << 19);
	LPC_UART3->IER|=(1<<0);
	LPC_PINCON->PINSEL0=((1 << 1)|(1<< 3));
	LPC_UART3->LCR|=(1<<7);
	LPC_UART3->DLL = 235;
	LPC_UART3->LCR&=~(1<<7);
	LPC_UART3->LCR = 0x03;
}

void Uart3_Transmit(unsigned char ch)
{
	LPC_UART3->THR=ch;	
}

void UART3_IRQHandler(void)
{	
	if(LPC_UART3->IIR==0x04)
	{
		ch=LPC_UART3->RBR;
		flag=1;
	}
}
