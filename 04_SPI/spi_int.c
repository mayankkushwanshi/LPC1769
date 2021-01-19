#include "LPC17xx.h"

/*
Master operation
The following sequence describes how one should process a data transfer with the SPI
block when it is set up to be the master. This process assumes that any prior data transfer
has already completed.
1. Set the SPI clock counter register to the desired clock rate.
2. Set the SPI control register to the desired settings.
3. Write the data to transmitted to the SPI data register. This write starts the SPI data
transfer.
4. Wait for the SPIF bit in the SPI status register to be set to 1. The SPIF bit will be set
after the last cycle of the SPI data transfer.
5. Read the SPI status register.
6. Read the received data from the SPI data register (optional).
7. Go to step 3 if more data is required to transmit.
*/

void delay(unsigned long time)
{
 	int i, j;
	for(i=0;i<1000;i++)
		for(j=0;j<time;j++);
}

void SPI_init()
{
	
	LPC_PINCON->PINSEL0|=(1<<31)/*SCK0*/;
	LPC_PINCON->PINSEL1|=(1<<1)/*SSEL0*/|(1<<3)/*MISO0*/|(1<<5)/*MOSI0*/;
	LPC_GPIO4->FIODIR0|=(1<<7);//Since Shift Register A;redy working in slave mode therefore no need to SSEL , insted using that pin for controlling ST_CP
	LPC_GPIO4->FIOSET0|=(1<<7);//ST_CP is storage register clock pulse
	LPC_SPI->SPCR=0x00;
	LPC_SPI->SPCR|=/*Master Mode*/(1<<5)/*LSB first*/|(1<<6)/*Interrupt enabled*/|(1<<7);
	LPC_SPI->SPCCR=0x0F;//As this input must be grater than 8 and a even number
	LPC_SPI->SPINT=(1<<0);
}

void SPI_data(unsigned char data)
{
	LPC_GPIO4->FIOSET0|=(1<<7);//to shift the data from storage reg of IC
	LPC_SPI->SPDR=data;//data
	while(!(LPC_SPI->SPSR&(1<<7)));//chesecking status if data is ready and transfer is complete
	LPC_GPIO4->FIOCLR0|=(1<<7);//to shift the data from storage reg of IC
}

void SPI_IRQHandler(void)
{
	static int j;
	static int i;
	LPC_SPI->SPINT=(1<<0);
	if(!(LPC_GPIO4->FIOSET0&(1<<7)))
		LPC_GPIO4->FIOSET0|=(1<<7);
	if(i==0)
	{
		LPC_GPIO2->FIOCLR1 |= (1<<3);
		i=1;
	}
	else
	{
		LPC_GPIO2->FIOSET1 |= (1<<3);
		i=0;
	}
	j=LPC_SPI->SPDR;// After every transmit the SPI also clocks in a data byte, which you have to clear with a dummy read.
	j=LPC_SPI->SPSR;//Read to clear flags
}

void spi_data(unsigned char data)
{
	LPC_GPIO4->FIOCLR0|=(1<<7);
	LPC_SPI->SPDR=data;
	LPC_GPIO4->FIOSET0|=(1<<7);
}

int main()
{
	SPI_init();
	LPC_GPIO2->FIODIR1 |= (1<<3);
	NVIC_EnableIRQ(SPI_IRQn);
	while(1)
	{
		spi_data(0x0f);
		delay(1000);
		spi_data(0xaa);
		delay(1000);
	}
}
