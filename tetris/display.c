/*
 * display.c
 *
 * Created: 26.09.2018 21:48:54
 *  Author: Alexander Velilyaev
 */ 
 
 #include "display.h"

 //Lcd rotated 90 degrees clockwise, x: [0: 47], y: [0: 83]
 void PutPixel(uint8_t x, uint8_t y, uint8_t value)
 {
	 SetPixel(y, 47 - x, value);
 }

 void DrawCell(uint8_t x, uint8_t y)
 {
	 x*= 4;
	 y*= 4;
	 x++;
	 y+=3;
	 for(uint8_t i = 0; i < 4; i++)
	 {
		 PutPixel(x + i, y, 1);
		 PutPixel(x + i, y + 3, 1);

		 if (i == 1 || i == 2)
		 {
			 PutPixel(x, y + i, 1);
			 PutPixel(x + 3,y + i, 1);
		 }
	 }
 }

 void RemoveCell(uint8_t x, uint8_t y)
 {
	 x*= 4;
	 y*= 4;
	 x++;
	 y+=3;
	 for(uint8_t i = 0; i < 4; i++)
	 {
		 PutPixel(x + i, y, 0);
		 PutPixel(x + i, y + 3, 0);

		 if (i == 1 || i == 2)
		 {
			 PutPixel(x, y + i, 0);
			 PutPixel(x + 3,y + i, 0);
		 }
	 }
 }

 void DrawBorders()
 {
	 for (uint8_t i = 3; i < 83; i++)
	 {
		 PutPixel(0,i,1);
		 PutPixel(41,i,1);
	 }

	 for (uint8_t i = 0; i < 42; i++)
	 {
		PutPixel(i,83,1);
	 }
 }

