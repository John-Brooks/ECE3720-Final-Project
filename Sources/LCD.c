#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include <stdio.h>
#include <math.h>
#include "LCD.h"
                                                                          
#define RS PORTK_BIT0_MASK  //RS pin of LCD
#define EN PORTK_BIT1_MASK  //EN pin of LCD

void LCD(float temp) {
  unsigned char LCDtemp[5];
  sprintf(LCDtemp, "%0.1f", temp);
  COMNWRT(0x88);//set cursor for temp 
  MSDelay(15); //give LCD some time
  DATAWRT(LCDtemp[0]);//display 
  MSDelay(15); //give LCD some time
  DATAWRT(LCDtemp[1]);//display 
  MSDelay(15); //give LCD some time
  DATAWRT(LCDtemp[2]);//display 
  MSDelay(15); //give LCD some time
  DATAWRT(LCDtemp[3]);//display 
}

void LCDsetup(void) 
{
  DDRK = 0xFF; //PORTK=output
  COMNWRT(0x33);//init. LCD 2 lines, 5x7 matrix
  MSDelay(250); //initialization hold
  COMNWRT(0x32);//init. LCD 2 lines, 5x7 matrix
  MSDelay(250); //initialization hold
  COMNWRT(0x28);//init. LCD 2 lines, 5x7 matrix
  MSDelay(15); //initialization hold
  COMNWRT(0x0C);//display on, cursor on
  MSDelay(15); //give LCD some time
  COMNWRT(0x01);//clear LCD
  MSDelay(15); //give LCD some time
  COMNWRT(0x06);//shift cursor right
  MSDelay(15); //give LCD some time
  COMNWRT(0x82);//cursor at line 1, pos. 2
  MSDelay(15); //give LCD some time
  DATAWRT('T');//display T
  MSDelay(15); //give LCD some time
  DATAWRT('E');//display E
  MSDelay(15); //give LCD some time
  DATAWRT('M');//display M
  MSDelay(15); //give LCD some time
  DATAWRT('P');//display P
  MSDelay(15); //give LCD some time
  DATAWRT(':');//display 
  MSDelay(15); //give LCD some time
  DATAWRT(' ');//display 
  MSDelay(15); //give LCD some time
  COMNWRT(0x8C);//cursor at line 1, pos. D
  MSDelay(15); //give LCD some time
  DATAWRT(223);//degree symbol
  MSDelay(15); //give LCD some time
  DATAWRT('C');//display 
  MSDelay(15); //give LCD some time
  
}
    
void COMNWRT(unsigned char command) //send com. to LCD
{
  PORTK = (command & 0xF0)>>2; //copy high nibble to LCD
  PORTK = PORTK & ~RS; //RS=0 for command
  PORTK = PORTK | EN; //EN=1 for high pulse
  PORTK = PORTK & ~EN; //EN=0 for H-to-L pulse
  PORTK = (command & 0x0F) << 2;//low nibble to LCD
  PORTK = PORTK | EN; //EB=1 for high pulse
  PORTK = PORTK & ~EN; //EN=0 for H-to-L pulse
}

void DATAWRT(unsigned char data) //write data to LCD
{
  PORTK = (data & 0xF0) >> 2; //copy high nibble to LCD
  PORTK = PORTK | RS; //RS=1 for data
  PORTK = PORTK | EN; //EB=1 for high pulse
  PORTK = PORTK & ~EN; //EN=0 for H-to-L pulse
  PORTK = (data & 0x0F) << 2; //low nibble to LCD
  PORTK = PORTK | RS;
  PORTK = PORTK | EN; //EB=1 for high pulse
  PORTK = PORTK & ~EN; //EN=0 for H-to-L pulse
}

void MSDelay (int ms) 
{
  int i,j;
  for(i=0; i < ms; i++)
    for (j=0; j < 4000; j++); //ms based on 8Mhz XTAL, 4MHz BUS freq
}