#include"lcd.h"

#include<string.h>


//function declarations

void UART1_CONFIG(void);

unsigned char UART1_RX(void);

void UART1_TX(unsigned char data);

void UART1_READ_TAG(void);

void UART1_CONFIG(void){

	PINSEL0 |=0X50000;

	U1LCR=0X83;

	U1DLL=97;

	U1DLM=0;

	U1LCR=0X03;

}


void UART1_TX(unsigned char data){

	while(((U1LSR>>5)&1)==0);

	U1THR=data;

}


unsigned char UART1_RX(void){

	while(((U1LSR>>0)&1)==0);

	return U1RBR;

}


void UART1_READ_TAG(void){

	int i=0;

	memset(buf,0,15);

	for(;i<12;i++){

		buf[i]=UART1_RX();

	}

	buf[i]='\0';

}
