/*
* display.h
*
* Created: 26.09.2018 21:52:03
*  Author: Alexander Velilyaev
*/


#ifndef DISPLAY_H_
#define DISPLAY_H_

#include <avr/io.h>
#include "libs/lcd_nokia5110/lcd_nokia5110.h"

#define SET_BIT(PORT, POS, VALUE) PORT = (PORT & ~(1 << POS)) | (VALUE << POS)
#define GET_BIT(PORT, POS) (PORT & (1 << POS)) >> POS

void DrawCell(uint8_t x, uint8_t y);

void RemoveCell(uint8_t x, uint8_t y);

void DrawBorders();

#endif /* DISPLAY_H_ */