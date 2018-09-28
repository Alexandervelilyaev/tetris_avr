/*
* tetris.c
*
* Created: 24.09.2018 23:40:22
* Author : Alexander Velilyaev
*/

#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/interrupt.h>

#include "display.h"
#include "buttons.h"
#include <stdlib.h>

#define WDTH 10
#define HGHT 20

typedef enum { I, J, L, O, Z, S, T } TetraType;

typedef struct {
	uint8_t x;
	uint8_t y;
} Point;

int8_t LeftData [7][4][4] = { { { 0, 1, 2, 3 }, { 0, -1, -1, -1 }, { 0, 1, 2, 3 }, { 0, -1, -1, -1 } }, { { 0, 1, -1, 3 }, { -1, -1, 2, 3 }, { -1, 1, 2, 3 }, { 0, -1, -1, 3 } }, { { 0, 1, 2, -1 }, { 0, 1, -1, -1 }, { 0, -1, 2, 3 }, { 0, -1, -1, 3 } }, { { 0, -1, 2, -1 }, { 0, -1, 2, -1 }, { 0, -1, 2, -1 }, { 0, -1, 2, -1 } }, { { 0, -1, 2, -1 }, { 0, -1, 2, 3 }, { 0, -1, 2, -1 }, { 0, -1, 2, 3 } }, { { 0, -1, 2, -1 }, { 0, 1, -1, 3 }, { 0, -1, 2, -1 }, { 0, 1, -1, 3 } }, { { 0, 1, -1, -1 }, { 0, 1, -1, 3 }, { 0, 1, -1, -1 }, { -1, 1, 2, 3 } } };
int8_t RightData [7][4][4] = { { { 0, 1, 2, 3 }, { -1, -1, -1, 3 }, { 0, 1, 2, 3 }, { -1, -1, -1, 3 } }, { { 0, 1, 2, -1 }, { 0, -1, -1, 3 }, { 0, -1, 2, 3 }, { -1, -1, 2, 3 } }, { { 0, 1, -1, 3 }, { 0, -1, -1, 3 }, { -1, 1, 2, 3 }, { -1, -1, 2, 3 } }, { { -1, 1, -1, 3 }, { -1, 1, -1, 3 }, { -1, 1, -1, 3 }, { -1, 1, -1, 3 } }, { { -1, 1, -1, 3 }, { 0, 1, -1, 3 }, { -1, 1, -1, 3 }, { 0, 1, -1, 3 } }, { { -1, 1, -1, 3 }, { 0, -1, 2, 3 }, { -1, 1, -1, 3 }, { 0, -1, 2, 3 } }, { { 0, -1, -1, 3 }, { -1, 1, 2, 3 }, { 0, -1, -1, 3 }, { 0, 1, -1, 3 } } };
int8_t DownData [7][4][4] = { { { -1, -1, -1, 3 }, { 0, 1, 2, 3 }, { -1, -1, -1, 3 }, { 0, 1, 2, 3 } }, { { -1, -1, 2, 3 }, { 0, 1, 2, -1 }, { 0, -1, -1, 3 }, { 0, 1, -1, 3 } }, { { -1, -1, 2, 3 }, { 0, -1, 2, 3 }, { 0, -1, -1, 3 }, { 0, 1, 2, -1 } }, { { -1, -1, 2, 3 }, { -1, -1, 2, 3 }, { -1, -1, 2, 3 }, { -1, -1, 2, 3 } }, { { 0, -1, 2, 3 }, { -1, 1, -1, 3 }, { 0, -1, 2, 3 }, { -1, 1, -1, 3 } }, { { -1, 1, 2, 3 }, { -1, 1, -1, 3 }, { -1, 1, 2, 3 }, { -1, 1, -1, 3 } }, { { 0, 1, -1, 3 }, { 0, -1, -1, 3 }, { -1, 1, 2, 3 }, { 0, -1, -1, 3 } } };
int8_t Coordinates [7][8] = { { 0, 0, 0, 1, 0, 2, 0, 3 }, { 1, 0, 1, 1, 1, 2, 0, 2 }, { 0, 0, 0, 1, 0, 2, 1, 2 }, { 0, 0, 1, 0, 0, 1, 1, 1 }, { 0, 0, 1, 0, 1, 1, 2, 1 }, { 1, 0, 2, 0, 0, 1, 1, 1 }, { 1, 1, 0, 0, 1, 0, 2, 0 } };
int8_t RotatingData[7][4][8] = { { { -1, 1, 0, 1, 1, 1, 2, 1 }, { 1, -1, 1, 0, 1, 1, 1, 2 }, { -1, 1, 0, 1, 1, 1, 2, 1 }, { 1, -1, 1, 0, 1, 1, 1, 2 } }, { { 1, 2, 0, 2, -1, 2, -1, 1 }, { -1, -2, -2, -2, -2, -1, -2, 0 }, { -1, 1, 0, 1, 1, 1, 1, 2 }, { 1, -1, 1, 0, 1, 1, 0, 1 } }, { { 0, 2, 0, 1, 1, 1, 2, 1 }, { 0, -2, 1, -2, 1, -1, 1, 0 }, { 0, 2, 1, 2, 2, 2, 2, 1 }, { 0, -2, 0, -1, 0, 0, 1, 0 } }, { { -1, -1, -1, -1, -1, -1, -1, -1 }, { -1, -1, -1, -1, -1, -1, -1, -1 }, { -1, -1, -1, -1, -1, -1, -1, -1 }, { -1, -1, -1, -1, -1, -1, -1, -1 } }, { { 1, 0, 1, 1, 0, 1, 0, 2 }, { -1, 0, 0, 0, 0, 1, 1, 1 }, { 1, 0, 1, 1, 0, 1, 0, 2 }, { -1, 0, 0, 0, 0, 1, 1, 1 } }, { { -1, 0, -1, 1, 0, 1, 0, 2 }, { 1, 0, 2, 0, 0, 1, 1, 1 }, { -1, 0, -1, 1, 0, 1, 0, 2 }, { 1, 0, 2, 0, 0, 1, 1, 1 } }, { { 0, 0, 1, -1, 1, 0, 1, 1 }, { 0, 0, -1, 1, 0, 1, 1, 1 }, { 0, 0, -1, -1, -1, 0, -1, 1 }, { 0, 0, -1, -1, 0, -1, 1, -1 } } };

Point Tetramino[4] = {{0,0}, {0,0}, {0,0}, {0,0}};
uint8_t Field[HGHT][WDTH];
uint8_t _state = 0;
uint8_t IsButtonClicked = 0;
int Score;
uint8_t CurrentType;

void InitTimer(void)
{
	TCCR1B |= (1<<WGM12) | (1<<CS12);
	TIMSK1 |= (1<<OCIE1A);
	OCR1A = F_CPU >> 8;
}

void Put(uint8_t value)
{
	for (uint8_t i = 0; i < 4; i++)
	{
		Field[Tetramino[i].y][Tetramino[i].x] = value;
	}
}

void Display()
{
	ClearBuffer();
	DrawBorders();

	for (uint8_t row = 0; row < HGHT; row++)
	{
		for (uint8_t col = 0; col < WDTH; col++)
		{
			if (Field[row][col] == 1)
			{
				DrawCell(col, row);
			}
		}
	}

	Flush();
}

void Move(int x, int y)
{
	Put(0);
	for (uint8_t i = 0; i < 4; i++)
	{
		Tetramino[i].x+= x;
		Tetramino[i].y+= y;
	}
	Put(1);
	Display();
}

uint8_t CanMoveLeft()
{
	for (uint8_t i = 0; i < 4; i++)
	{
		int index = LeftData[(int)CurrentType][_state][i];
		if (index > -1 && (Tetramino[index].x == 0 || Field[Tetramino[index].y][Tetramino[index].x - 1] == 1))
		{
			return 0;
		}
	}
	return 1;
}

uint8_t CanMoveRight()
{
	for (uint8_t i = 0; i < 4; i++)
	{
		int index = RightData[(int)CurrentType][_state][i];
		if (index > -1 && (Tetramino[index].x == WDTH - 1 || Field[Tetramino[index].y][Tetramino[index].x + 1] == 1))
		{
			return 0;
		}
	}
	return 1;
}

void SwitchState()
{
	_state = _state == 3 ? 0 : _state + 1;
}

void OnButtonLeft_Press()
{
	if (CanMoveLeft())
	{
		IsButtonClicked = 1;
		Move(-1, 0);
		IsButtonClicked = 0;
	}
}

void OnButtonRight_Press()
{
	if (CanMoveRight())
	{
		IsButtonClicked = 1;
		Move(1, 0);
		IsButtonClicked = 0;
	}
}

void ResetField()
{
	Score = 0;
	for (uint8_t i = 0; i < HGHT; i++)
	{
		for (uint8_t j = 0; j < WDTH; j++)
		{
			Field[i][j] = 0;
		}
	}
}

void GenerateTetramino()
{
	_state = 0;
	CurrentType = rand() % 7;
	for (uint8_t i = 0; i < 4; i++)
	{
		Tetramino[i].x = Coordinates[(int)CurrentType][i * 2] + WDTH / 2 - 1;
		Tetramino[i].y = Coordinates[(int)CurrentType][i * 2 + 1];
	}

	for (uint8_t i = 0; i < 4; i++)
	{
		if (Field[Tetramino[i].y][Tetramino[i].x] == 1)
		{
			//GameOver();
		}
	}
}

uint8_t CanMoveDown()
{
	for (uint8_t i = 0; i < 4; i++)
	{
		int index = DownData[(int)CurrentType][_state][i];
		if (index > -1 && (Tetramino[index].y >= HGHT - 1 || Field[Tetramino[index].y + 1][Tetramino[index].x] == 1))
		{
			return 0;
		}
	}
	return 1;
}

uint8_t GetSum(uint8_t row)
{
	uint8_t sum = 0;
	for (uint8_t i = 0; i < WDTH; i++)
	{
		sum += Field[row][i];
	}
	return sum;
}

void ShiftField(uint8_t row)
{
	while (row > 0 && GetSum(row - 1) > 0)
	{
		for (uint8_t i = 0; i < WDTH; i++)
		{
			Field[row][i] = Field[row - 1][i];
			Field[row - 1][i] = 0;
		}
		row--;
	}
}

void CheckFullLines()
{
	if (!CanMoveDown())
	{
		for (uint8_t row = 0; row < HGHT; row++)
		{
			if (GetSum(row) == WDTH)
			{
				Score += 10;
				for (uint8_t col = 0; col < WDTH; col++)
				{
					Field[row][col] = 0;
				}
				ShiftField(row);
				Display();
			}
		}
	}
}

uint8_t Contains(Point point)
{
	for (uint8_t i = 0; i < 4; i++)
	{
		if (Tetramino[i].x == point.x && Tetramino[i].y == point.y)
		{
			return 1;
		}
	}

	return 0;
}

void MakeMove()
{
	if (CanMoveDown())
	{
		Move(0, 1);
	}
	else
	{
		CheckFullLines();
		GenerateTetramino();
	}
}

void Rotate()
{
	Point TestTetramino[] = {{0,0},{0,0},{0,0},{0,0}};

	int x = Tetramino[0].x;
	int y = Tetramino[0].y;

	for (uint8_t i = 0; i < 4; i++)
	{
		TestTetramino[i].x = x + RotatingData[(int)CurrentType][_state][i * 2];
		TestTetramino[i].y = y + RotatingData[(int)CurrentType][_state][i * 2 + 1];
	}

	if (CurrentType != 3)
	{
		uint8_t canRotate = 1;

		for (uint8_t i = 0; i < 4; i++)
		{
			if (TestTetramino[i].x < 0 || TestTetramino[i].x >= WDTH || TestTetramino[i].y < 0 || TestTetramino[i].y >= HGHT || (Field[TestTetramino[i].y][TestTetramino[i].x] == 1 && !Contains(TestTetramino[i])))
			{
				canRotate = 0;
			}
		}

		if (canRotate)
		{
			SwitchState();
			Put(0);
			
			for (uint8_t i = 0; i < 4; i++)
			{
				Tetramino[i] = TestTetramino[i];
			}
			
			Put(1);
			Display();
		}
	}
}

void OnButtonUp_Press()
{
	IsButtonClicked = 1;
	Rotate();
	IsButtonClicked = 0;
}

void OnButtonDown_Press()
{
	IsButtonClicked = 1;
	MakeMove();
	IsButtonClicked = 0;
}

ISR(TIMER1_COMPA_vect)
{
	if (IsButtonClicked == 0)
	{
		MakeMove();
	}
}

int main(void)
{
	InitLCD('B', 1, 2, 3, 4, 5);
	InitButtons();
	InitTimer();

	ResetField();
	GenerateTetramino();
	Put(1);
	Display();

	sei();
	while (1)
	{
		PollButtons();
	}
}
