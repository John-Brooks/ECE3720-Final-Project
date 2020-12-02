#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "spi_thermocouple.h"

//SPICR1 register flags (page 532)
#define SPIE_FLG 0x80
#define SPE_FLG 0x40 
#define SPTIE_FLG 0x20
#define MSTR_FLG 0x10
#define CPOL_FLG 0x08
#define CPHA_FLG 0x04
#define SSOE_FLG 0x02
#define LSBFE_FLG 0x01

//SPICR2 register flags (page 532)
#define MODFEN_FLG 0x10
#define BIDIROE_FLG 0x08
#define SPISWAI_FLG 0x02
#define SPC0_FLG 0x01

//SPISR (SPI Status Regiester page 533)
#define SPIF_FLG 0x80 //new data received
#define SPTEF_FLG 0x20 //write buffer empy, must be read before transmit
#define MODE_FLG 0x10

#define NUM_DATA_BYTES 4

int bytes[NUM_DATA_BYTES];

void InitSPI()
{
  DDRS |= 0xE0;
  PTS |= 0x80;


	//we can tune this value lower later. Setting this to 1K now so that we run SPI very slow
	//and hopefully reliably. I don't feel like getting out the oscilloscope to see how fast it can run.
	SPI0BR = 0x76; //setup baud rate divisor to 1024.
	//SPI0BR = 0x07; //setup baud rate divisor to 256.
  //SPI0BR = 0x00; //setup baud rate divisor to 256.
	//Enable SPI module and set to master
	SPI0CR1 = SPE_FLG | MSTR_FLG;
	SPI0CR2 = 0x00;// MODFEN_FLG;

	//Set internal pullups
	WOMS = 0x00;
}

void ReadFromSPI()
{
	int i; 
	PTS &= ~0x80; 
	for(i = 0; i < NUM_DATA_BYTES; i++)
	{
		//wait for the tx buffer to be empty
		while(!(SPI0SR & SPTEF_FLG));
    	                                                                        
		//transmit dummy data that will trigger the module to Rx data from the TC amp.
		SPI0DR = 0;

		//wait for the data to be received.
		while(!(SPI0SR & SPIF_FLG));

		bytes[i] = SPI0DR;
	}
	PTS |= 0x80;
}

float GetTemperature()
{
	float temperature;

	ReadFromSPI();
	
	temperature = (bytes[0] & 0x7F) * 16;
	temperature += (bytes[1] >> 2) * 0.25;
	temperature *= (bytes[0] & 0x80) * -1;

	return temperature;
}