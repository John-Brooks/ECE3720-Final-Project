#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "pi_control.h"
#include "spi_thermocouple.h"
#include "LCD.h" 
#include "FanPWM.h"

#define UPDATE_RATE_S 2.0
#define CLOCK_FREQ 24000000
#define PRESCALER 128

void InitTimer()
{
	TSCR1 = 0x80; //Enable Timer
	TSCR2 = 0x07; //Set prescaler to 128
	TIOS = 0x40; //0100 0000 (channel 6 is outputs)
}
void PLLInit()
{
   SYNR = 2;
   REFDV = 0;
   PLLCTL = 0x60;  //PLLON = 1, High Bandwidth is selected in PLLCTL regsiter
   while(!(CRGFLG & 0x08)); // wait until the PLL locks into the target frequency  
   CLKSEL = 0x80; //PLLSEL = 1 FOR THE CLKSEL Regsiter
}
long ConvertTimeToCycles(float time, int prescaler)
{
	float tick_per_second = (float)CLOCK_FREQ / (float)prescaler;
	return (long)(time * tick_per_second);
}
void DelayTicks(long ticks)
{
	unsigned int full_cycles = ticks / 65536;
	unsigned int partial_cycle = ticks % 65536;
	unsigned int current_cycle = 0;
	TC6 = TCNT;
	while (current_cycle++ < full_cycles)
	{
		TFLG1 = 0x40; //reset counter 6 flag
		//wait out a full cycle of the 16-bit timer
		while((TFLG1 & 0x40) == 0)
			continue;
	}

	//wait out the partial cycle
	TC6 += partial_cycle;
	TFLG1 = 0x40; //reset counter 6 flag
	while((TFLG1 & 0x40) == 0)
		continue;
}


void main(void) {
	float temperature;
	int update_counter = 0;
	Controller_t controller;
	InitController(&controller);
	InitTimer();
	PLLInit();
	InitSPI();
	InitFanPWM();
	LCDsetup();

	EnableInterrupts;
	for(;;)
	{
	  temperature = GetTemperature(); 
		RunController(&controller, temperature, UPDATE_RATE_S);
		SetFanSpeed(0.95);
		
		if (update_counter % 5 == 0) {
		  LCD(temperature);
		  DelayTicks(ConvertTimeToCycles(UPDATE_RATE_S - LCD_UPDATE_DELAY_S, PRESCALER));
		} else {
		  DelayTicks(ConvertTimeToCycles(UPDATE_RATE_S, PRESCALER));  
		}
		
		update_counter++;
	}  
}
