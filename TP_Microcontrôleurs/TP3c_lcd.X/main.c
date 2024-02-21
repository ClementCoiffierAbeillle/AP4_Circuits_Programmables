/*
 * File: main.c
 * Author: clément.coiffier & jeremie.warconsin
 * 
 * Ecrire un programme qui affiche « Hello, World ! » sur l?écran LCD
 */

#include "configbits.h" // Bits de configuration
#include <xc.h>         // Definition des registres specifiques au uC
#define DELAY 1
#define _XTAL_FREQ 8000000
#include "../Ressources/lcd.h"
#include "../Ressources/spi.h"

void init() {
    //Assignation RX/TX
    RC6PPSbits.RC6PPS = 0x14; //10100 p.154
    RXPPSbits.RXPPS = 0x17; //10111 p.153

    //Configuration des pins RC6&RC7
    TRISCbits.TRISC6 = 0; //définie C6 en sortie
    TRISCbits.TRISC7 = 1; //définie C7 en entrée
    ANSELCbits.ANSC7 = 0; //numerique

    //Configuration du Baudrate à 9600 bauds
    TX1STAbits.SYNC = 0; //p.363
    TX1STAbits.BRGH = 0; //low speed p.368
    BAUD1CONbits.BRG16 = 0;
    SP1BRGHbits.SP1BRGH = 0; //high bits
    SP1BRGLbits.SP1BRGL = 0x0C; //low bits

    //Activation du module EUSART
    RC1STAbits.SPEN = 1; //serial port enable
    TX1STAbits.TXEN = 1; //transmit enable
    RC1STAbits.CREN = 1; //enables receiver  
    
}
void main(void) {
    /* Code d'initialisation */
    init();
    SPI_InitializePins();
    LCD_InitializePins();
    SPI_Initialize();
    LCD_Initialize();
    LCD_Clear();
    LCD_GoTo(0,0);
    LCD_WriteString("Hello, World !");
            
    while (1) {
        //Utilisation du module EUSART
    
    }
}