//Headers used
#include<LPC21XX.H>
#include<string.h>
#include"lcd.h"

//Function declarations
void DISPLAY(void);
char COMPARE_ID(void);
void DISPLAY_NAME(unsigned char *);
void GSM_INIT(void);
void GSM_SEND_MSG(unsigned char *,unsigned char *);
void UART1_CONFIG(void);
unsigned char UART1_RX(void);
void UART1_FLUSH(void);
void UART1_TX(unsigned char data);
void UART1_READ_TAG(void);
void UART0_CONFIG(void);
unsigned char UART0_RX(void);
void UART0_TX(unsigned char);
void GSM_COMMAND(unsigned char *);
void UART0_STR(unsigned char *);

//Global varibales
char buf[15];
char hash[5];
typedef struct{
	unsigned char ID[15];
	unsigned char NAME[25];
	unsigned char PHNO[15];
}EMP;

	EMP E1={"060067D80EB7","PRIYA","+919092964708"};
	EMP E2={"0600674E6C43","NAGIREDDY","+919951661493"};

int main(){
	LCD_INIT();
	LCD_STR("SYS IS BOOTING");
	UART0_CONFIG();
	UART1_CONFIG();
	UART1_FLUSH();
	memset(hash,0,5);
	delay_ms(15000);
	LCD_COMMAND(0X01);
	LCD_STR("GSM IS INIT...");
	GSM_INIT();
	while(1){
		DISPLAY();
		UART1_FLUSH();
		UART1_READ_TAG();
		switch(COMPARE_ID()){
			case 1:
					if(hash[0]==0){
					DISPLAY_NAME(E1.NAME);
					GSM_SEND_MSG(E1.PHNO,"YOUR PRESENCE IS REGISTERED");
					hash[0]=1;
					break;
					}else{
					LCD_COMMAND(0X01);
					LCD_COMMAND(0X80);
					LCD_STR("ALREADY MARKED");
					delay_ms(2000);
					LCD_COMMAND(0X01);
					break;
					}
			case 2:
					if(hash[1]==0){
					DISPLAY_NAME(E2.NAME);
					GSM_SEND_MSG(E2.PHNO,"YOUR PRESENCE IS REGISTERED");
					hash[1]=1;
					break;
					}
					else{
					LCD_COMMAND(0X01);
					LCD_COMMAND(0X80);
					LCD_STR("ALREADY MARKED");
					delay_ms(2000);
					LCD_COMMAND(0X01);
					break;
					}
			default :
					DISPLAY_NAME("INVALID CARD");
					break;
		}
		delay_ms(1000);
		UART1_FLUSH();		
	} 
}

void UART1_FLUSH(void){
	unsigned char dummy;
	while(U1LSR&1)
	dummy=U1RBR;
}
void DISPLAY(void){
	LCD_COMMAND(0X01);
	LCD_COMMAND(0X80);
	LCD_STR("SCAN YOUR CARD");
	delay_ms(1000);
}

char COMPARE_ID(void){
	if(strcmp(buf,E1.ID)==0)return 1;
	if(strcmp(buf,E2.ID)==0)return 2;
	return 0;
}

void DISPLAY_NAME(unsigned char * name){
	LCD_COMMAND(0X01);
	LCD_COMMAND(0X80);
	LCD_STR(name);
	delay_ms(2000);
}

void GSM_SEND_MSG(unsigned char *num,unsigned char *str){
	GSM_COMMAND("AT+CMGS=\"");
	delay_ms(500);
	GSM_COMMAND(num);
	GSM_COMMAND("\"\r\n");
	delay_ms(2000);
	GSM_COMMAND(str);
	UART0_TX(0x1A);
	delay_ms(5000);	
}

/*void GSM_INIT(void){
	GSM_COMMAND("AT\r\n");                       
	delay_ms(1000);
	GSM_COMMAND("ATE0\r\n");                    
	delay_ms(1000);
	GSM_COMMAND("AT+CMEE=1\r\n");                
	delay_ms(1000);
	GSM_COMMAND("AT+CMGF=1\r\n");                
	delay_ms(1000);
	GSM_COMMAND("AT+CSCA=\"+919390972785\",145\r\n"); 
	delay_ms(1000);
	GSM_COMMAND("AT+CSMP=17,167,0,0\r\n");       
	delay_ms(1000);
}*/
void GSM_INIT(void){
	GSM_COMMAND("AT\r\n");           delay_ms(1000);
	GSM_COMMAND("ATE0\r\n");         delay_ms(1000);
	GSM_COMMAND("AT+CMEE=1\r\n");    delay_ms(1000);
	GSM_COMMAND("AT+CMGF=1\r\n");    delay_ms(1000);
	GSM_COMMAND("AT+CSCS=\"GSM\"\r\n"); delay_ms(1000);
}

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

void UART0_CONFIG(void){
	PINSEL0 |= 0X05;                         // P0.0 = TXD0, P0.1 = RXD0
	U0LCR = 0X83;
	U0DLL = 8;                              // 115200 baud at PCLK = 15 MHz
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

void GSM_COMMAND(unsigned char *data){
	while(*data){
		UART0_TX(*data++);
	}
}

void UART0_STR(unsigned char *str){
	while(*str){
		UART0_TX(*str++);
	}
}
