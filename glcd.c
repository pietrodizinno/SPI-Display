/*
=================================================================================
 Name        : PCD8544.c
 Version     : 0.1

 Copyright (C) 2016 Volkan UGUR NeuralKey

================================================================================
This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.
================================================================================
 */
#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>

#define CS           15 /*          */
#define RST          14 /*              */
#define CD           16 /*    */

#define SCK          12 /*    */
#define SDA          13

#define DISPLAY_ON           0xaf   //  Display on
#define DISPLAY_OFF          0xae   //  Display off
#define SET_ADC              0xa1   //  Reverse disrect (SEG131-SEG0)
#define CLEAR_ADC            0xa0   //  Normal disrect (SEG0-SEG131)
#define REVERSE_DISPLAY_ON   0xa7   //  Reverse display : 0 illuminated
#define REVERSE_DISPLAY_OFF  0xa6   //  Normal display : 1 illuminated
#define ENTIRE_DISPLAY_ON    0xa5   //  Entire dislay   Force whole LCD point
#define ENTIRE_DISPLAY_OFF   0xa4   //  Normal display
#define SET_BIAS             0xa3   //  bias 1   1/7 bais
#define CLEAR_BIAS           0xa2   //  bias 0   1/9 bais
#define RESET                0xe2 	//   system reset
#define SET_SHL              0xc8   // SHL 1,COM63-COM0
#define CLEAR_SHL            0xc0   // SHL 0,COM0-COM63

#define Start_column	0x00
#define Start_page		0x00
#define	StartLine_set	0x00


//**	GENERAL PURPOSE		**//
unsigned int say,hamal;
unsigned char temp,i;
unsigned char ID[2];
unsigned char INP[4];

//////////////////////////////////////////////////////


void SetByte(unsigned char CMD);
void WriteByte(unsigned char CMD);
void Set_Contrast(unsigned char Value);


void Delay(unsigned long T){
														while (T!=0)
															T--;
													}

void SetByte(unsigned char CMD){
digitalWrite(CS,LOW);
Delay(500);
digitalWrite(CD,LOW);
Delay(2500);
for(i=0;i<8;i++){
	               digitalWrite(SCK,LOW);
                 temp=CMD;
								 digitalWrite(SDA,temp&0x80?1:0);  //0x80
                 Delay(15000);
	               digitalWrite(SCK,HIGH);
                 temp=CMD<<1;  // <<
	               CMD=temp;
                }
Delay(2500);
digitalWrite(CS,HIGH);
Delay(500);
digitalWrite(CD,LOW);
}

void WriteByte(unsigned char CMD){
digitalWrite(CS,LOW);
Delay(500);
digitalWrite(CD,HIGH);
Delay(2500);
for(i=0;i<8;i++){
	               digitalWrite(SCK,LOW);
                 temp=CMD;
	               digitalWrite(SDA,temp&0x80?1:0);  //0x80
                 Delay(15000);
	               digitalWrite(SCK,HIGH);
                 temp=CMD<<1;  // <<
	               CMD=temp;
                }

Delay(2500);
digitalWrite(CS,HIGH);
Delay(500);
digitalWrite(CD,LOW);
}



void Set_Contrast(unsigned char mod){
                                     if(mod>0x3f)mod=0;//0..63
	                                   SetByte(0x81);
	                                   SetByte(mod);
	                                   return;
}


void Dispay_Line(unsigned char line){//initial
                                     line|=0x40;
                                     SetByte(line);
	                                   return;
}
// Set page address 0~15
void Page_Address(unsigned char add){
                                     add=0xb0|add;
                                     SetByte(add);
	                                   return;
}

void Column_Address(unsigned char add){
                                       SetByte((0x10|(add>>4)));
	                                     SetByte((0x0f&add+0x04));
	                                     return;
}

void Power_Control(unsigned char vol){
                                      SetByte((0x28|vol));
	                                    return;
}

/*  Regulor resistor select
**            1+Rb/Ra  Vo=(1+Rb/Ra)Vev    Vev=(1-(63-a)/162)Vref   2.1v
**            0  3.0       4  5.0(default)
**            1  3.5       5  5.5
**            2  4         6  6
**            3  4.5       7  6.4
*/
void Resistor_Select(unsigned char r){
                                      SetByte((0x20|r));
	                                    return;
}


void LCD_Initial(void){



										pinMode(CS, OUTPUT);
										pinMode(RST, OUTPUT);
										pinMode(CD, OUTPUT);
										pinMode(SCK, OUTPUT);
										pinMode(SDA, OUTPUT);


									 digitalWrite(CS,LOW);
									 Delay(5000);
									 digitalWrite(RST,LOW);
									 Delay(300);
									 digitalWrite(RST,HIGH);
									 Delay(500);
	                 SetByte(RESET);
									 Delay(250);
	           	     SetByte(CLEAR_BIAS);
									 Delay(250);
	                 SetByte(SET_ADC);
	                 Delay(250);
									 SetByte(CLEAR_SHL);
	                 Delay(250);
							     SetByte(0x40);//?
									 Delay(250);
	           	     SetByte(CLEAR_BIAS);
	                 Delay(250);
									 SetByte(0x2f);//?
	                 Delay(250);
									 SetByte(0x81);//?
	                 Delay(250);
									 SetByte(0x08);//?
	                 Delay(250);
									 Power_Control(0x07);
	                 Delay(250);
									 Resistor_Select(0x06);
	                 Delay(250);
									 Set_Contrast(15);//deniyorum
	                 Delay(250);
									 Dispay_Line(0x00);
	                 Delay(250);
									 SetByte(DISPLAY_ON);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char pic[]={//resim
	                        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xC0,0xE0,0xF0,0xF8,0xF0,0xE0,0xC0,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3C,0x3C,0x3C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3C,0x3C,0x3C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
													0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xC0,0xE0,0xF0,0xF8,0xFC,0x7E,0xBF,0xDF,0xEF,0xF7,0xFB,0x01,0xFB,0xF7,0xEF,0xDF,0xBF,0x7E,0xFC,0xF8,0xF0,0xE0,0xC0,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xE0,0xF8,0xFC,0x7E,0x3E,0x1F,0x1F,0x1F,0x1F,0x1F,0x3E,0xFE,0xFC,0xF8,0xC0,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x3F,0xFE,0xFC,0xF0,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0xF0,0xFC,0xFE,0x3F,0x1F,0x1F,0x1F,0x1F,0x7E,0xFE,0xF8,0xE0,0x00,0x00,0x00,0x00,0x00,0x00,
													0x00,0xC0,0xE0,0xF0,0xF8,0xFC,0xFE,0xFF,0xFF,0xFF,0xF7,0xFB,0x7D,0x3E,0x1F,0x0F,0xFF,0xFF,0xFF,0xFF,0x00,0xFF,0xFF,0xFF,0xFF,0x0F,0x1F,0x3E,0x7D,0xFB,0xF7,0xFF,0xFF,0xFF,0xFE,0xFC,0xF8,0xF0,0xE0,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0x01,0x00,0x00,0x00,0x00,0x00,0xF0,0xF0,0xF0,0xF0,0xF1,0xF1,0xF0,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0x7C,0x7C,0x7C,0xFC,0xFC,0xFC,0xFC,0xBE,0x3F,0x1F,0x07,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x80,0x83,0x87,0x0F,0x1E,0x1C,0x1C,0x3C,0x38,0x38,0x79,0xF1,0xE1,0x80,0x00,0x00,0x00,0x00,0x00,
													0x00,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x01,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0x00,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x01,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x0F,0x3F,0x7F,0xFC,0xF8,0xF0,0xF0,0xF0,0xF1,0xF1,0xF9,0xF9,0x7F,0x3F,0x1F,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x03,0x07,0x3F,0xFE,0xF8,0xC0,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x07,0x1F,0x7F,0xFE,0xF8,0xF0,0xF0,0xF0,0xF0,0xF8,0xFC,0xFF,0x7F,0x1F,0x00,0x00,0x00,0x00,0x00,
													0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0x00,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x60,0x67,0x67,0x7F,0x3F,0x3D,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
													0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0x00,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x04,0x1C,0x7C,0xFC,0xE0,0x80,0xE0,0xFC,0x7C,0x1C,0x04,0x00,0x00,0x00,0x00,0xE0,0xFC,0x7C,0x7C,0xFC,0xE0,0x00,0x00,0x00,0x00,0x00,0xFC,0xFC,0xFC,0x0C,0x0C,0x1C,0xFC,0xF8,0xF0,0x00,0x00,0x0C,0x0C,0xFC,0xFC,0xFC,0x0C,0x0C,0x00,0x00,0x00,0xFC,0xFC,0xFC,0x3C,0xF8,0xE0,0x00,0x00,0xFC,0xFC,0xFC,0x00,0x00,0x0C,0x0C,0xFC,0xFC,0xFC,0x0C,0x0C,0x00,0x00,0x00,0x0C,0x0C,0x0C,0x0C,0x8C,0xEC,0xFC,0xFC,0x3C,0x1C,0x00,0x00,0x00,0x00,0x00,0x00,
													0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0x80,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0xFF,0xFF,0xFF,0x01,0x00,0x00,0x00,0x00,0xC0,0xF8,0xFF,0x7F,0x1F,0x18,0x18,0x1F,0x7F,0xFF,0xF8,0xC0,0x00,0x00,0xFF,0xFF,0xFF,0x06,0x06,0x07,0x07,0x03,0x01,0x00,0x00,0xC0,0xC0,0xFF,0xFF,0xFF,0xC0,0xC0,0x00,0x00,0x00,0xFF,0xFF,0xFF,0x00,0x01,0x07,0x1F,0x7C,0xFF,0xFF,0xFF,0x00,0x00,0xC0,0xC0,0xFF,0xFF,0xFF,0xC0,0xC0,0x00,0x00,0x00,0xE0,0xF0,0xFC,0xFF,0xDF,0xC7,0xC3,0xC0,0xC0,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,
													0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};




void Write_Picture(unsigned char pic[]){
    unsigned char i,j;
    for(i=0;i<8;i++){
                     SetByte(0x40);
                     SetByte(0xb0|i);
                     SetByte(0x10);
                     SetByte(0x04);
                     for(j=0;j<128;j++){WriteByte(pic[i*128+j]);}

										 }
									 }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char num[]={ //numerik 0...9
	                        0x00,0xF8,0xFC,0x04,0x04,0xFC,0xF8,0x00,0x00,0x03,0x07,0x04,0x04,0x07,0x03,0x00,
													0x00,0x00,0x08,0xFC,0xFC,0x00,0x00,0x00,0x00,0x00,0x04,0x07,0x07,0x04,0x00,0x00,
													0x00,0x18,0x1C,0x84,0xC4,0x7C,0x38,0x00,0x00,0x06,0x07,0x05,0x04,0x04,0x04,0x00,
													0x00,0x08,0x0C,0x24,0x24,0xFC,0xD8,0x00,0x00,0x02,0x06,0x04,0x04,0x07,0x03,0x00,
													0x80,0xE0,0x70,0x18,0xFC,0xFC,0x00,0x00,0x00,0x01,0x01,0x05,0x07,0x07,0x04,0x00,
													0x00,0x7C,0x7C,0x24,0x24,0xE4,0xC4,0x00,0x00,0x03,0x07,0x04,0x04,0x07,0x03,0x00,
													0x00,0xF0,0xF8,0x6C,0x24,0xEC,0xCC,0x00,0x00,0x03,0x07,0x04,0x04,0x07,0x03,0x00,
													0x00,0x0C,0x0C,0xC4,0xFC,0x3C,0x04,0x00,0x00,0x00,0x00,0x07,0x07,0x00,0x00,0x00,
													0x00,0x98,0xFC,0x64,0x64,0xFC,0x98,0x00,0x00,0x03,0x07,0x04,0x04,0x07,0x03,0x00,
													0x00,0x78,0xFC,0x84,0xC4,0xFC,0xF8,0x00,0x00,0x06,0x06,0x04,0x06,0x03,0x01,0x00};






void Write_number(unsigned char *n,unsigned char k,unsigned char station_dot){

unsigned char i;
			for(i=0;i<8;i++)
				{
				WriteByte(*(n+16*k+i));
				}
		    Page_Address(Start_page+1)	;
        Column_Address(Start_column+station_dot*8);
			  for(i=8;i<16;i++)
				{
				WriteByte(*(n+16*k+i));
				}
}


void Display_Numara(unsigned char line,unsigned int numara){
unsigned char sayi[5];

	 sayi[5]=((numara%100000)/10000);
	 sayi[4]=((numara%10000)/1000);
	 sayi[3]=((numara%1000)/100);
	 sayi[2]=((numara%100)/10);
	 sayi[1]=(numara%10);


   Dispay_Line(line);

  Column_Address(Start_column+0*8);
	Page_Address(Start_page);
  Write_number(num,sayi[5],0);
	Column_Address(Start_column+1*8);
	Page_Address(Start_page);
	Write_number(num,sayi[4],1);
  Column_Address(Start_column+2*8);
	Page_Address(Start_page);
  Write_number(num,sayi[3],2);
	Column_Address(Start_column+3*8);
	Page_Address(Start_page);
	Write_number(num,sayi[2],3);
  Column_Address(Start_column+4*8);
	Page_Address(Start_page);
  Write_number(num,sayi[1],4);

}

void main(void){

		printf("Orange Pi Custom Display test\n");
		printf("==================+++====================\n");

		// check wiringPi setup
		if (wiringPiSetup() == -1)
		{
		printf("wiringPi-Error\n");
			exit(1);
		}


	LCD_Initial();
	printf("LCD INIT....\n");
	Delay(31000);
	printf("LCD INIT OK.\n");
	SetByte(REVERSE_DISPLAY_OFF);
	Delay(3100);
  SetByte(ENTIRE_DISPLAY_OFF);
	Delay(5100);
  Write_Picture(pic);
	printf("WRITE PIC OK.\n");
	Delay(500000);
	printf("WRITE OK.\n");
	hamal=0;
  Display_Numara(1,hamal);
	printf("WRITE NUMBER.\n");

	while(1){}



}
