//PIC16F877A Configuration Bit Settings
//'C' source line config statements
//CONFIG 
#pragma config FOSC = HS // Oscillator Selection bits (HS oscillator) 
#pragma config WDTE = OFF // Watchdog Timer Enable bit (WDTdisabled) 
#pragma config PWRTE = ON // Power-up Timer Enable bit (PWRT enabled) 
#pragma config BOREN = ON // Brown-out Reset Enable bit (BOR enabled) 
#pragma config LVP = OFF // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming) 
#pragma config CPD = OFF // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off) 
#pragma config WRT = OFF // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF // Flash Program Memory Code Protection bit (Code protection off)
// #pragma config statements should precede project file includes. 
//Use project enums instead of \#define for ON and OFF.

#include <xc.h> 
#define _XTAL_FREQ 20000000 
#define ECHO RB0
#define TRIG RB1 
#define MOTOR1 RC3 
#define MOTOR2 RC5

//TIMER INTERRUPT
void __interrupt() timer_isr(void){ 
if(TMR1IF==1){
MOTOR1 = 0; 
MOTOR2 = 1; 
__delay_ms(500); 
MOTOR2= 0; 
TMR1IF = 0;
} 
}

void main(void){

//initializing input output pins 
TRISB0= 1; //ECO PIN
TRISB1= 0; //TRIGGER PIN 
//TRISB2= 1; //ECO PIN 
TRISC3= 0; //BLUE LED-MOTOR1 
TRISC5= 0; //RED LED- MOTOR 2 

//T1C0N REGISTER 
T1CON = 0X20; //1:4 PRESCALAR WITH INTERNAL CLOCK 

//INTCON REGISTER 
GIE = 1; //GLOBAL INTERRUPTS ENABLED 
PEIE= 1; //PHERIPHERAL INTERRUPT ENABLED 

//PIR1 REGISTER 
TMR1IF = 0; //NO OVERFLOW 

//PIE REGISTER 
TMR1IE = 1; //TIMER1 INTERRUPT ENABLED

MOTOR1 = 0;
MOTOR2 = 0;

int distance;
int time_taken;

while(1){
    
    TMR1H= 0;
    TMR1L= 0;
    
    //INITIALIZING US SENSOR
    TRIG = 1;
    __delay_ms(10);
    TRIG = 0;
    
    while(ECHO==0);
    TMR1ON= 1;
    while(ECHO==1);
    TMR1ON= 0;
    
    time_taken= (TMR1L | (TMR1H<<8));
    distance= (0.0272*time_taken)/2;
    
    //time_taken= time_taken*0.8;
    //INITIALIZING FUNCTION OF SENSOR1
    if(distance<10){
        MOTOR1= 1;
        MOTOR2= 0;
    }else{
        MOTOR1= 0;
        MOTOR2= 0;
    }
    //INITIALIZING FUNCTION OF SENSOR2
    if(10<=distance && distance<20){
        MOTOR1= 1;
        MOTOR2= 0;
    }else{
        MOTOR1= 0;
        MOTOR2= 0;
    }
    //INITIALIZING FUNCTION OF SENSOR3
    if(20<=distance && distance<30){
        TMR1IF=1;
    }
    
}

return;