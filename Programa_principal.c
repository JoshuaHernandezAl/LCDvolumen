/*
 * File:   Programa_principal.c
 * Author: Joshua Hernandez
 *
 * Created on 6 de mayo de 2021, 01:31 PM
 */


#include <xc.h>
#include "Configuracion.h"
#include "LCD_libreria.h"

const char wels[]={     0b00011111,
                        0b00011111,
                        0b00011111,
                        0b00011111,
                        0b00011111,
                        0b00011111,
                        0b00011111,
                        0b00000000,0
};

void LCD_Init(void){
    OpenXLCD(FOUR_BIT & LINES_5X7 );
    while(BusyXLCD()); // LCD esta ocupado
    WriteCmdXLCD(0x06); //Mover el cursor a la derecha
    WriteCmdXLCD(0x0C); //Desactivar el cursor
}
void __interrupt(high_priority) JOSHUAint0(void);
unsigned char cuenta=0;
int p=0,i=0;
void main(void) {    
    INTCONbits.GIE=0;//Evitamos la interrupcion por error
    
    TRISBbits.RB0=1;//configuramos a RB0 como entrada
    TRISBbits.RB1=1;//configuramos a RB1 como entrada
    
   /* TRISBbits.RB4=0;
    TRISBbits.RB5=0;
    TRISBbits.RB6=0;
    TRISBbits.RB7=0;*/
    
    INTCONbits.INT0IE=1;//habilitamos la interrupcion de int0 atraves de la terminal rb0
    INTCON2bits.INTEDG0=1;//asignamos un flanco ascendente para la interrupcion
    INTCONbits.INT0IF=0;//Inicializamos en cero la bandera 
    
    RCONbits.IPEN=0;//Inhablitamos prioridades, usamos el modo de compatibilidad
    INTCONbits.GIE=1;//apartir de aqui se habilitan las interrupciones
    
    LATB=0x00;//el valor inicial en la parte alta del puerto B sera cero
    LCD_Init();

    WriteCmdXLCD(BORRAR_LCD);
    LCD_gotoXY(0,0);
    putrsXLCD("VOLUMEN:");
    __delay_ms(100);
    LCD_WriteChr_CGRAM(wels,0);
    __delay_ms(500);
    /**/
    
    while(1){
        //LATB=cuenta<<4;
        if(p>0){
            LCD_gotoXY(0,0);
            putrsXLCD("VOLUMEN:");
            __delay_ms(100);
            for(i=0;i<p;i++){
                LCD_gotoXY(1,i);
                putcXLCD(0);
                __delay_ms(200);
            }
        }
    }
    return;
}
void __interrupt(high_priority) JOSHUAint0(void){
    if(INTCONbits.INT0IF){//conrpueba el esta de verdad en la bandera
        __delay_us(100);//Ponemos 100 us para omitir la lectura de rebotes
        if(PORTBbits.RB1 != PORTBbits.RB0){
            //cuenta++;//esta variable debe de ser de tipo global
            p++;
        }else if(PORTBbits.RB1 == PORTBbits.RB0){
            //cuenta--;
            p--;
        }
    }
    WriteCmdXLCD(BORRAR_LCD);
    INTCONbits.INT0IF=0;//poniendo la bandera a cero para que no regrese a la interrupcion
}
