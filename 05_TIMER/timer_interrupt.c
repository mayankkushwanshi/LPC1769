//polling
/*#include "LPC17xx.h"
#define PRESCALE 72000

void initTimer(void);
void delayMS(unsigned int milliseconds);

int main (void)
{	 
	SystemInit();   		
	initTimer(); //Initialize Timer0

	LPC_PINCON->PINSEL4&=~((1 <<22)|(1<<23));
	LPC_GPIO2->FIODIR1 |= (1<<3);

	while(1)
	{
		LPC_GPIO2->FIOSET1 |= (1<<3);
		delayMS(1000);
		LPC_GPIO2->FIOCLR1 |= (1<<3);
		delayMS(1000);
	}
}

void initTimer(void)
{
   	LPC_SC->PCONP=((1<<1)|(1<<15));
	LPC_SC->PCLKSEL0=(1<<2);
    LPC_TIM0->CTCR = 0x00;
    LPC_TIM0->PR = PRESCALE-1; //(Value in Decimal!) - Increment T0TC at every 60000 clock cycles
                     //Count begins from zero hence subtracting 1
                     //60000 clock cycles @60Mhz = 1 mS

	LPC_TIM0->TCR = 0x02; //Reset Timer
}

void delayMS(unsigned int milliseconds) //Using Timer0
{
	LPC_TIM0->TCR = 0x02; //Reset Timer

    LPC_TIM0->TCR = 0x01; //Enable timer
   
    while(LPC_TIM0->TC < milliseconds); //wait until timer counter reaches the desired delay
   
    LPC_TIM0->TCR = 0x00; //Disable timer
}*/

//interrupt
#include "LPC17xx.h"
#define PRESCALE 72000

uint32_t flag=0;
void initTimer(void);
void TIMER0_IRQHandler(void);

int main (void)
{	 
	SystemInit();   		
	initTimer(); //Initialize Timer0

	LPC_PINCON->PINSEL4&=~((1 <<22)|(1<<23));
	LPC_GPIO2->FIODIR1 |= (1<<3);

	NVIC_EnableIRQ(TIMER0_IRQn);       //Call API to enable interrupt in NVIC
    //NVIC_SetPriority(PriorityN); //Set priority if needed

	while(1)
	{
	}
}

void initTimer(void)
{
    LPC_SC->PCONP=((1<<1)|(1<<15));
	LPC_SC->PCLKSEL0=(1<<2);
    LPC_TIM0->CTCR = 0x0;
    LPC_TIM0->PR = PRESCALE-1; //(Value in Decimal!) - Increment T0TC at every 60000 clock cycles
                     //Count begins from zero hence subtracting 1
                     //60000 clock cycles @60Mhz = 1 mS

	LPC_TIM0->TCR = 0x02; //Reset Timer
	LPC_TIM0->TCR=(1<<0);// Timer Enable
	LPC_TIM0->MCR = ((1<<0)|(1<<1));
	LPC_TIM0->MR0 = 1000;
}

void TIMER0_IRQHandler(void)
{
	LPC_TIM0->IR=(1<<0);//to reset Interrupt flag for match channel 0 or MR0

	if(flag==0)
	{
		flag=1;
		LPC_GPIO2->FIOSET1 |= (1<<3);
	} 
	else
	{
		flag=0;
		LPC_GPIO2->FIOCLR1 |= (1<<3);
	}
	LPC_TIM0->TC=0x00;//Reset the timer counter	
}
