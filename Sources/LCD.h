#pragma once

#define LCD_UPDATE_DELAY_S 0.770

void LCD(float temp);
void COMNWRT(unsigned char);
void DATAWRT(unsigned char);
void MSDelay(int);
void LCDsetup();