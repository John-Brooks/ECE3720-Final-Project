#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "pi_control.h"
#include "spi_thermocouple.h"

#define UPDATE_RATE_S 0.5
#define CLOCK_FREQ 24000000
#define PRESCALER 128

//sets the fan speed PWM (0.0 - 1.0)
void SetFanSpeed(float pwm)
{

}

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
	Controller_t controller;
	InitController(&controller);
	InitTimer();
	PLLInit();
	InitSPI();

	EnableInterrupts;
	for(;;)
	{
	  	temperature = GetTemperature(); 
		RunController(&controller, temperature, UPDATE_RATE_S);
		SetFanSpeed(controller.output);
		//DelayTicks(ConvertTimeToCycles(UPDATE_RATE_S, PRESCALER));
	}  
	
	for(;;) {
		_FEED_COP(); /* feeds the dog */
	} /* loop forever */
/* please make sure that you never leave main */
}
