#include <lpc17xx.h>

void delay(void);

int main(void)
{
	LPC_GPIO0->FIODIR = 0xFFFFFF; // Configure pins 0 to 3 on Port 0 as Output
	
	while(1)
	{
		LPC_GPIO0->FIOSET = 0xFFFFFF; // Output HIGH
		delay();
		LPC_GPIO0->FIOCLR = 0xFFFFFF; // Output LOW
		delay();
	}
	//return 0; // normally this wont execute
}	

void delay(void) //Hardcoded delay function
{
	int count,i=0;
	for(count=0; count < 6000000; count++) // You can edit this as per your needs
	{
		i++; // something needs to be here else compiler will remove the for loop!
	}
}
