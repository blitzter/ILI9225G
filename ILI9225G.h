/*
 * ILI9225G.h
 *
 *  Created on: Jun 24, 2013
 *      Author: blitzter
 */
#include <Arduino.h>

#ifndef ILI9225G_H_
#define ILI9225G_H_

#define WHITE          0xFFFF
#define BLACK          0x0000
#define BLUE           0x001F
#define BLUE2          0x051F
#define RED            0xF800
#define MAGENTA        0xF81F
#define GREEN          0x07E0
#define CYAN           0x7FFF
#define YELLOW         0xFFE0
#define ORANGE		   0xF900
#define FONT_WIDTH      5
#define FONT_HEIGHT     7

class ILI9225G {
public:
	int cs;
	int rs;
	int rst;
	int mosi;
	int sclk;

	ILI9225G(int cs, int rs, int rst, int mosi, int sclk);
	virtual ~ILI9225G();

	void begin();
	void writeCommand(unsigned short wr_cmd_a, unsigned short wr_cmd_b);
	void writeData(unsigned short wr_cmd_a);
	void writeDataFast(unsigned short wr_cmd_a);
	void writeRegister(unsigned short wr_cmd_a);
	void initializeLCD();
	void clearScreen(unsigned short color);
	void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
	void printChar(char casc, unsigned char postion_x, unsigned char postion_y,
			unsigned short f_clr, unsigned short b_clr, unsigned short size);
	void printNumber(int s, int digits, unsigned char x, unsigned char y,
			unsigned short f_clr, unsigned short b_clr, unsigned short size);
	void printString(char *s, unsigned char x, unsigned char y,
			unsigned short f_clr, unsigned short b_clr, unsigned short size);
	void printString(String s, unsigned char x, unsigned char y,
			unsigned short f_clr, unsigned short b_clr, unsigned short size);

};

#endif /* ILI9225G_H_ */
