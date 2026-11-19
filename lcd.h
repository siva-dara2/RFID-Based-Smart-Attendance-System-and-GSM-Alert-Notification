#include<lpc21xx.h>
#include"timers.h"
#define LCD_D 0xff<<13
#define RS 1<<10
#define E 1<<11

void LCD_INIT(void);
void LCD_STR(unsigned char *p);
void LCD_COMMAND(unsigned char);
void LCD_DATA(unsigned char);
void LCD_INT(int);
void LCD_FLOAT(float);

void LCD_INIT(void){
	IODIR0=LCD_D|RS|E;
	delay_ms(20);
	LCD_COMMAND(0x01);
	LCD_COMMAND(0x02);
	LCD_COMMAND(0x0c);
	LCD_COMMAND(0x38);
}

void LCD_COMMAND(unsigned char cmd){
	IOCLR0 = LCD_D;
	IOSET0 = cmd<<13;
	IOCLR0 = RS;
	IOSET0 = E;
	delay_ms(2);
	IOCLR0 = E;
}

void LCD_DATA(unsigned char data){
	IOCLR0 = LCD_D;
	IOSET0 = data<<13;
	IOSET0 = RS;
	IOSET0 = E;
	delay_ms(2);
	IOCLR0 = E;
}

void LCD_INT(int n){
	int i=0;
	int arr[15];
	if(n==0){
		LCD_DATA('0');
	}else if(n<0){
		LCD_DATA('-');
		n=(~n)+1;
	}
	while(n){
		arr[i++]=n%10;
		n/=10;
	}
	i--;
	while(i>=0){
		LCD_DATA(arr[i]+'0');
		i--;
	}
}

void LCD_STR(unsigned char *p){
	while(*p){
		if(*p=='\n'){
			LCD_COMMAND(0xC0);
			p++;
		}
		LCD_DATA(*p++);
	}
}

void LCD_FLOAT(float fun){

	LCD_INT((int)fun);
	LCD_DATA('.');
	fun=fun-(int)fun;
	fun*=10;
	while(fun){
	LCD_INT((int)fun);
	fun=fun-(int)fun;
	fun*=10;
	}
}
