#include<lpc17xx.h>

void uart_init(void);
void uart_send(char*);
char uart_rcv(void);
char a[20];
int main()
{
	char *a;
	int i;
	uart_init();
	while(1)
	{
			for(int i=0;i<10;i++)
		{
			a[i]=uart_rcv();
			if(a[i] == 'n')
			break;		
		}
		
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
	while(*str++!= 'n')
	{
	while(!(LPC_UART0->LSR & (1<<5)));
		LPC_UART0->THR = *str;	
	}
	
}
char uart_rcv(void)
{
	char a;
	
	while(!(LPC_UART0->LSR & (1<<0)));
	a= LPC_UART0->RBR;
	return a;
}
	