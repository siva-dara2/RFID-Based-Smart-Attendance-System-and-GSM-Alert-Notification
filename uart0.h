//headers used

#include<LPC21XX.h>


//function declarations

void UART0_CONFIG(void);

unsigned char UART0_RX(void);

void UART0_TX(unsigned char);

void GSM_COMMAND(char *);




void UART0_CONFIG(void){

	PINSEL0 |= 0X05;                         // P0.0 = TXD0, P0.1 = RXD0

	U0LCR = 0X83;

	U0DLL = 97;                              // 9600 baud at PCLK = 15 MHz

	U0DLM = 0;

	U0LCR = 0X03;

}


void UART0_TX(unsigned char data){

	while(((U0LSR>>5)&1)==0);

	U0THR = data;

}


unsigned char UART0_RX(void){

	while(((U0LSR>>0)&1)==0);

	return U0RBR;

}


void GSM_COMMAND(char *data){

	while(*data){

		UART0_TX(*data++);

	}

}

