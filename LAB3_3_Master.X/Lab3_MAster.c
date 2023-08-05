//*****************************************************************************
/*
 * File:   main.c
 * Author: Pablo
 * Ejemplo de implementación de la comunicación SPI 
 * Código Maestro
 * Created on 10 de febrero de 2020, 03:32 PM
 */
//*****************************************************************************
//*****************************************************************************
// Palabra de configuración
//*****************************************************************************
// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)
// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits Write protection off)



// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

//*****************************************************************************
// Definición e importación de librerías
//*****************************************************************************
#include <xc.h>
#include <stdint.h>
#include "SPI.h"
#include "LCD.h"
//*****************************************************************************
// Definición de variables
//*****************************************************************************
#define _XTAL_FREQ 8000000
//*****************************************************************************
// Definición de funciones para que se puedan colocar después del main de lo 
// contrario hay que colocarlos todas las funciones antes del main
//*****************************************************************************
void setup(void);
void uint8ToString(uint8_t num, char *str);
void floatToStr(float value, char* buffer, int precision);
char buffer[4];
char buffer2[4];
char buffer3[4];
uint8_t slave1;  
uint8_t slave1_cont; 
uint8_t slave2; 
//*****************************************************************************
// Código Principal
//*****************************************************************************
void main(void) {
    setup();
    //*************************************************************************
    // Loop infinito
    //*************************************************************************
    while(1){
        
       PORTCbits.RC2 = 0;       //Slave Select
       __delay_ms(1); 
       spiWrite(1);
       slave1 = spiRead();
       __delay_ms(1);
       PORTCbits.RC2 = 1;       //Slave Deselect 
       
       __delay_ms(250);
       
       
       
       
       
       PORTCbits.RC1 = 0;       //Slave Select
       __delay_ms(1); 
       spiWrite(PORTB);
       slave2 = spiRead();
       __delay_ms(1);
       PORTCbits.RC1 = 1;       //Slave Deselect 
       
       __delay_ms(250);
       
       PORTCbits.RC2 = 0;       //Slave Select
       __delay_ms(1); 
       spiWrite(0);
       slave1_cont = spiRead();
       __delay_ms(1);
       PORTCbits.RC2 = 1;       //Slave Deselect 
       
       __delay_ms(250);
       
      
       uint8ToString(slave1,buffer);
       
       if (slave1<100){
           Lcd_Set_Cursor(2,1);
           Lcd_Write_String("   ");
       }
       Lcd_Set_Cursor(2,1);
       Lcd_Write_String(buffer);
       
       
       
        uint8ToString(slave2,buffer2);
       
       if (slave2<100){
           Lcd_Set_Cursor(2,8);
           Lcd_Write_String("   ");
       }
       Lcd_Set_Cursor(2,8);
       Lcd_Write_String(buffer2);
       
       
       
       uint8ToString(slave1_cont,buffer3);
       
       if (slave1_cont<100){
           Lcd_Set_Cursor(2,14);
           Lcd_Write_String("   ");
       }
       Lcd_Set_Cursor(2,14);
       Lcd_Write_String(buffer3);
       
      
    }
    return;
}
//*****************************************************************************
// Función de Inicialización
//*****************************************************************************
void setup(void){
     // CONFIGURACION DEL OSCILADOR
    OSCCONbits.IRCF2 = 1;
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF0 = 1; // 8MHZ
    OSCCONbits.SCS = 1;  // OSCILADOR INTERNO 
    ANSEL = 0;
    ANSELH = 0;
    TRISC2 = 0;
    TRISC1 = 0;
    TRISB = 0;
    TRISD = 0;
    PORTB = 0;
    PORTD = 0;
    PORTCbits.RC2 = 1;
    spiInit(SPI_MASTER_OSC_DIV4, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
    Lcd_Init();
    Lcd_Clear();
    Lcd_Set_Cursor(1,1);
    Lcd_Write_String("S1:");
    Lcd_Set_Cursor(1,8);
    Lcd_Write_String("S2:");
    Lcd_Set_Cursor(1,14);
    Lcd_Write_String("S3:");
}

void uint8ToString(uint8_t num, char *str) {
    uint8_t temp = num;
    int8_t i = 0;

    // Handle the case when the input number is 0 separately
    if (temp == 0) {
        str[0] = '0';
        str[1] = '\0';
        return;
    }

    // Extract digits from the number and store them in reverse order
    while (temp > 0) {
        str[i++] = '0' + (temp % 10);
        temp /= 10;
    }

    // Add null terminator
    str[i] = '\0';

    // Reverse the string to get the correct order of digits
    int8_t left = 0;
    int8_t right = i - 1;
    while (left < right) {
        char tempChar = str[left];
        str[left] = str[right];
        str[right] = tempChar;
        left++;
        right--;
    }
}