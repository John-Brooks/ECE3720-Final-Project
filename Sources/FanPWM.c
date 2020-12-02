#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "FanPWM.h"

unsigned int y; //PWM variable

void PWMDuty(unsigned int y){  //PWM  
  PWMDTY0 = y >> 8;           //highest 8 bits of the duty cycle
  PWMDTY1 = y & 0x00FF;     //lowest 8 bits of the duty cycle
}

void Delay(unsigned int itime) //PWM 
{
  unsigned int i, j;
  for(i=0;i<itime;i++)
    for(j=0;j<4000;j++);
}

void SetFanSpeed(float duty_cycle)
{ 
  	y = 120 * duty_cycle;
	  ATD0CTL5 = 0x87; 
  	//while(!(ATD0STAT0 & ATD0STAT0_SCF_MASK));
    //set Y to any number between 0 and 120 to get PWM signal out it
    PWMDuty(y);   
}

void InitFanPWM()
{
	DDRP = 0xFF;
	PWMPRCLK = 0x03;    
	// 0x04 to divide by 2^3 = 8
	// Clock A = 24MHz/8 = 3MHz
	PWMPER0 = 120 >> 8; //highest 8 bits of the Period
	PWMPER1 = 120 & 0xFF; //lowest 8  bits of the period
	// 3MHz / 120 = 25KHz exactly what we need for the Fan  

	ATD0CTL2 = 0xC0;    //Power up, Fast Flag clear
	Delay(1);
	ATD0CTL3 = 0x08;    //1 conversion non FIFO
	ATD0CTL4 = 0x85;    //8-bit, 2MHz ATD clk, 2 clks for the 2nd phase

	PWMCTL = 0x1C;      //Concatenate PWM4 and PWM5 enable the (16) bit mode
	PWMPOL = 0x02;      //when we use 16 bits, we need only to initialized 1 to the upper channel 
	PWMCAE = 0x00;      //
	PWMCLK = 0x00;
	//PWMCNT01 = 0;
	PWME = 0x02; //enable the upper channel
}
