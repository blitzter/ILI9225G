/*
 * ILI9225G.cpp
 *
 *  Created on: Jun 24, 2013
 *      Author: blitzter
 */

#include <avr/pgmspace.h>
#include "glcdfont.c"
#include "ILI9225G.h"
//#include "Arduino.h"
#include <SPI.h>

ILI9225G::ILI9225G(int cs, int rs, int rst, int mosi, int sclk) {
	this->cs = cs;
	this->rs = rs;
	this->rst = rst;
	this->mosi = mosi;
	this->sclk = sclk;
}

ILI9225G::~ILI9225G() {
	SPI.end();
}

void ILI9225G::begin() {
	pinMode(cs, OUTPUT);
	pinMode(rs, OUTPUT);
	SPI.begin();
	SPI.setClockDivider(SPI_CLOCK_DIV8);
	SPI.setBitOrder(MSBFIRST);
	SPI.setDataMode(SPI_MODE0);
	initializeLCD();
}

void ILI9225G::writeCommand(unsigned short wr_cmd_a, unsigned short wr_cmd_b) {
	writeRegister(wr_cmd_a);
	writeData(wr_cmd_b);
}

void ILI9225G::writeData(unsigned short wr_cmd_a) {
	digitalWrite(rs, HIGH);
	digitalWrite(cs, LOW);
	SPI.transfer(wr_cmd_a >> 8);
	SPI.transfer(wr_cmd_a & 0xFF);
	digitalWrite(cs, HIGH);
}

void ILI9225G::writeDataFast(unsigned short wr_cmd_a) {
	//digitalWrite(cs,LOW);
	SPI.transfer(wr_cmd_a >> 8);
	SPI.transfer(wr_cmd_a & 0xFF);
	//digitalWrite(cs,HIGH);
}

void ILI9225G::writeRegister(unsigned short wr_cmd_a) {
	digitalWrite(rs, LOW);
	digitalWrite(cs, LOW);
	SPI.transfer(wr_cmd_a >> 8);
	SPI.transfer(wr_cmd_a & 0xFF);
	digitalWrite(cs, HIGH);
}

void ILI9225G::initializeLCD() {
	digitalWrite(cs, LOW);
	digitalWrite(rst, LOW);

	delay(50);
	digitalWrite(rst, HIGH);

	writeCommand(0x28, 0x00CE);
	writeCommand(0x01, 0x011C);

	writeCommand(0x03, 0x1030);  //1018

	writeCommand(0x07, 0x0017);
	delay(50);

	writeCommand(0x11, 0x1000);   //0808
	writeCommand(0x20, 0x0000);
	writeCommand(0x21, 0x0000);
	delay(50);
	writeCommand(0x30, 0x0000);  // set_windows_x_y()
	writeCommand(0x31, 0x00DB);  //DB
	writeCommand(0x32, 0x0000);
	writeCommand(0x33, 0x0000);
	writeCommand(0x34, 0x00DB);  //DB
	writeCommand(0x35, 0x0000);

	writeCommand(0x36, 0x00AF);
	writeCommand(0x37, 0x0000);
	writeCommand(0x38, 0x00DB);  //DB
	writeCommand(0x39, 0x0000);
	delay(50);
	writeCommand(0x02, 0x0000); // Gamma Command
	delay(50);
//	writeCommand(0x0d, 0x2100); // Gamma Command

	writeCommand(0x61, 0x0103);
	writeCommand(0xE8, 0x1000);
	writeCommand(0xB0, 0x0812);
	writeCommand(0x0B, 0x0000);
	writeCommand(0x0D, 0x0000);
	writeCommand(0xB1, 0x0404);
	writeCommand(0x62, 0x0019);
	delay(50);
	writeCommand(0xE8, 0x0100);

	writeCommand(0x50, 0x0007);
	writeCommand(0x51, 0x0708);
	writeCommand(0x52, 0x0D0A);
	writeCommand(0x53, 0x0404);
	writeCommand(0x54, 0x0007);
	writeCommand(0x55, 0x0706);
	writeCommand(0x56, 0x0D0A);
	writeCommand(0x57, 0x0404);
	writeCommand(0x58, 0x0000);
	writeCommand(0x59, 0x0000);

	writeCommand(0x20, 0x0000);
	writeCommand(0x21, 0x0000);
	writeCommand(0x07, 0x1017);
	delay(80);
}

void ILI9225G::clearScreen(unsigned short color) {
	unsigned char i, j;

	writeCommand(0x36, 0x00AF); 								 //x start point
	writeCommand(0x37, 0x0000); 								//y start point
	writeCommand(0x38, 0x00DB); 								   //x end point
	writeCommand(0x39, 0x0000); 								//y end point

	// Set the initial value of address Counter
	writeCommand(0x20, 0x0000);
	writeCommand(0x21, 0x0000);

	writeRegister(0x22);									//write data to GRAM
	digitalWrite(rs, HIGH);
	digitalWrite(cs, LOW);
	for (i = 0; i < 220; i++) {
		for (j = 0; j < 176; j++) {
			writeDataFast(color);
		}
	}
	digitalWrite(cs, HIGH);
}

void ILI9225G::printChar(char casc, unsigned char postion_x,
		unsigned char postion_y, unsigned short f_clr, unsigned short b_clr,
		unsigned short size) {
	register unsigned short i, j, l = 0;

	int b = 0;
	int *p = 0;
	int k = 1;
	uint8_t pos;

	writeCommand(0x37, postion_x); 					//x start point
	writeCommand(0x39, postion_y); 					//y start point

	writeCommand(0x36, (postion_x) + (FONT_WIDTH * size) - 1);	//x end point
	writeCommand(0x38, (postion_y) + (FONT_HEIGHT * size) + (size - 1));//y end point

	// Set the initial value of address Counter
	writeCommand(0x20, postion_x);
	writeCommand(0x21, postion_y);

	writeRegister(0x22);								//write data to GRAM

	uint8_t line[6];
	//if (i == 5) 
	line[5] = 0x0;
	//else 
	for (int8_t i = 0; i < 5; i++) {
		line[i] = pgm_read_byte(font+(casc*5)+i);
	}
	pos = line[0];
	for (int8_t j = 0; j < 8; j++) {
		for (int8_t k = 0; k < size; k++) {
			for (int8_t i = 0; i < 5; i++) {
				if (line[i] & 0x1) {
					for (int8_t l = 0; l < size; l++) {
						writeData(f_clr);
					}
				} else {
					for (int8_t l = 0; l < size; l++) {
						writeData(b_clr);
					}
				}
			}
		}
		for (int8_t i = 0; i < 5; i++) {
			line[i] >>= 1;
		}
	}
}

void ILI9225G::printNumber(int s, int digits, unsigned char x, unsigned char y,
		unsigned short f_clr, unsigned short b_clr, unsigned short size) {
	String str = String(s);
	//int len = str.length();
	while (str.length() < digits) {
		str = String(0) + str;
	}
	printString(str, x, y, f_clr, b_clr, size);
}

void ILI9225G::printString(char *s, unsigned char x, unsigned char y,
		unsigned short f_clr, unsigned short b_clr, unsigned short size) {
	while (*s) {

		printChar(*s, x, y, f_clr, b_clr, size);

		x += (FONT_WIDTH * size + 1 * ((size + 1) / 2));
		if (x >= 176) {
			x = 0;
			y += (FONT_HEIGHT * size + 1 * ((size + 1) / 2));
			if (y >= 220) {
				y = 0;
			}
		}

		s++;
	}
}

void ILI9225G::printString(String s, unsigned char x, unsigned char y,
		unsigned short f_clr, unsigned short b_clr, unsigned short size) {
	for (int8_t i = 0; i < s.length(); i++) {

		printChar(s.charAt(i), x, y, f_clr, b_clr, size);

		x += (FONT_WIDTH * size + 1 * ((size + 1) / 2));
		if (x >= 176) {
			x = 0;
			y += (FONT_HEIGHT * size + 1 * ((size + 1) / 2));
			if (y >= 220) {
				y = 0;
			}
		}
	}
}

void ILI9225G::fillRect(int16_t x, int16_t y, int16_t w, int16_t h,
		uint16_t color) {
	unsigned char i, j;

	writeCommand(0x37, x); 					//x start point
	writeCommand(0x39, y); 					//y start point

	writeCommand(0x36, (x) + (w));		    //x end point
	writeCommand(0x38, (y) + (h));	        //y end point

	// Set the initial value of address Counter
	writeCommand(0x20, 0x0000);
	writeCommand(0x21, 0x0000);

	writeRegister(0x22);									//write data to GRAM
	//digitalWrite(rs, HIGH);
	//digitalWrite(cs, LOW);
	for (i = 0; i < 220; i++) {
		for (j = 0; j < 176; j++) {
			writeDataFast(color);
		}
	}
	//digitalWrite(cs, HIGH);	
}
