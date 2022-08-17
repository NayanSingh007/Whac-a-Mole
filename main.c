#include <at89c5131.h>
#include "lcd.h"		//Header file with LCD interfacing functions
#include "serial.h"	//C file with UART interfacing functions


char Score[2];
char HighScore[2];
char value[8];
int b = 5;
int a = 0;
int STOP = 0;
int Value10 = 400;
int t;
int SInt = 0;
int HInt = 0;
int i,j;
code unsigned char KM[] = "qwertyuiasdfghjk";

void lcd_ready(void)
{
	 lcd_cmd(0x80);
	 lcd_write_string("   Get Ready  ");
	 msdelay(2000);
	 lcd_cmd(0x01);
}

void lcd_score(void)
{
	 lcd_cmd(0x80);
	 lcd_write_string("Score:     ");
	 int_to_string(SInt,Score);
	 lcd_write_string(Score);
	 lcd_cmd(0x0C0);
	 lcd_write_string("High Score:");
	 int_to_string(HInt,HighScore);
	 lcd_write_string(HighScore);
	 msdelay(3000);
	 lcd_cmd(0x01);
}

void Star1(void)
{	
	lcd_cmd(0x80);
	lcd_write_string("* * * * * * * * ");
	lcd_cmd(0x0C0);
	lcd_write_string("* * * * * * * * ");
	msdelay(3000);
	lcd_cmd(0x01);
}

void Star2(int n)
{
	lcd_cmd(0x80);
	for(i=0;i<8;i++){
		if( i == n)
		{
			lcd_write_string("m ");
		}
		else
		{
			lcd_write_string("* ");
		}
	}
	lcd_cmd(0x0C0);
	for(j=8;j<16;j++){
		if( j == n)
		{
			lcd_write_string("m ");
		}
		else
		{
			lcd_write_string("* ");
		}
	}	
}

void MolePos(int *m)
{
	/*a = 0;
	t = *m;
	a = ((t >> 3)&1)^((t & 1));
	a = a << 1 ;
	a = a | ((t >> 3)&1);
	a = a << 1;
	a = a | ((t >> 2)&1);
	a = a << 1;
	a = a | ((t >> 1)&1);
	*m = (a);//%16;*/	
	t = *m;
	a = ((t ^ (t << 3)) & 0x08) | ((t >> 1) & 0x07);
	*m = (a + *m)%16;
}

void GamePlay(void)
{	
	lcd_ready();
	Star1();
	Star2(b);
	TR0 = 1;
}

void Timer0(void) interrupt 1
{
		TR0 = 0;
		if (Value10 == 0){
			STOP = 1;
		}
		else {
			TH0 = 0x3C;
			TL0 = 0xB0;
			Value10--;
			TR0 = 1;
		}		
}

//Main function
void main(void)
{
	unsigned char ch=0;
	
	
	//Call initialization functions
	lcd_init();
	uart_init();
	
	//lcd_ready();
	GamePlay();
	
	
	while(1)
	{
				ch = receive_char();
				
				if (KM[b] == ch)
				{
					SInt++;
					MolePos(&b);
					Star2(b);
				
				}
				if (STOP == 1){
					STOP = 0;
					if(SInt > HInt){
						HInt = SInt;
					}
					Value10 = 400;
					lcd_score();
					SInt = 0;
					GamePlay();
				}
	}
}
