#include "LPC17xx.h"


int main (void)
{	    		
	unsigned int i, j;

	LPC_PINCON->PINSEL4&=~((1 <<22)|(1<<23));
	LPC_GPIO2->FIODIR1 |= (1<<3);

	while(1)
	{
		LPC_GPIO2->FIOSET1 |= (1<<3);
		for(i=0;i<5000;i++)
			for(j=0;j<100;j++);
		LPC_GPIO2->FIOCLR1 |= (1<<3);
		for(i=0;i<5000;i++)
			for(j=0;j<100;j++);
	}
}
