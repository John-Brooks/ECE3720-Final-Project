#pragma once

//sets up the SPI registers to begin reading thermocouple data
void InitSPI();

//read temperature over SPI from the thermocouple amplifier
float GetTemperature();