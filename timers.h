#include<LPC21XX.H>
void delay_ms(int);
void delay_sec(int);

void delay_ms(int n){
        T0TCR = 0x01;
        T0PR  = 15000-1;
        while(T0TC<n);
        T0TCR = 0x03;
        T0TCR = 0x00;
}
void delay_sec(int n){
        T0TCR = 0x01;
        T0PR  = 15000000-1;
        while(T0TC<n);
        T0TCR = 0x03;
        T0TCR = 0x00;
}
