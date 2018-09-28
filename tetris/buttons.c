/*
* buttons.c
*
* Created: 26.09.2018 22:33:08
*  Author: Alexander Velilyaev
*/

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "buttons.h"

uint8_t isClickedUp = 0;
uint8_t isClickedDown = 0;
uint8_t isClickedLeft = 0;
uint8_t isClickedRight = 0;

void InitButtons()
{
	DDRC &= ~(1 << BTN_UP);
	DDRC &= ~(1 << BTN_LEFT);
	DDRC &= ~(1 << BTN_RIGHT);
	DDRC &= ~(1 << BTN_DOWN);

	PORTC |= (1 << BTN_UP);
	PORTC |= (1 << BTN_LEFT);
	PORTC |= (1 << BTN_RIGHT);
	PORTC |= (1 << BTN_DOWN);
}

void OnButtonUp_Unpress()
{
}

void OnButtonDown_Unpress()
{
}

void OnButtonLeft_Unpress()
{
}

void OnButtonRight_Unpress()
{
}

void PollButtons()
{
	_delay_ms(20);
	if (!(PINC & (1 << BTN_UP) ) && isClickedUp == 0)
	{
		isClickedUp = 1;
		OnButtonUp_Press();
	}
	if (PINC & (1 << BTN_UP) && isClickedUp == 1)
	{
		isClickedUp = 0;
		OnButtonUp_Unpress();
	}
	if (!(PINC & (1 << BTN_DOWN) ) && isClickedDown == 0)
	{
		isClickedDown = 1;
		OnButtonDown_Press();
	}
	if (PINC & (1 << BTN_DOWN) && isClickedDown == 1)
	{
		isClickedDown = 0;
		OnButtonDown_Unpress();
	}
	if (!(PINC & (1 << BTN_LEFT) ) && isClickedLeft == 0)
	{
		isClickedLeft = 1;
		OnButtonLeft_Press();
	}
	if (PINC & (1 << BTN_LEFT) && isClickedLeft == 1)
	{
		isClickedLeft = 0;
		OnButtonLeft_Unpress();
	}
	if (!(PINC & (1 << BTN_RIGHT) ) && isClickedRight == 0)
	{
		isClickedRight = 1;
		OnButtonRight_Press();
	}
	if (PINC & (1 << BTN_RIGHT) && isClickedRight == 1)
	{
		isClickedRight = 0;
		OnButtonRight_Unpress();
	}
}