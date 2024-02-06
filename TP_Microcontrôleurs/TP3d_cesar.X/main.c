#include "configbits.h" // Bits de configuration
#include <xc.h>         // Definition des registres specifiques au uC
#define DELAY 1
#define _XTAL_FREQ 8000000
#include "../Ressources/lcd.h"
#include "../Ressources/spi.h"
#include "string.h"
#include "stdio.h"

#define BOUTON1 PORTBbits.RB0
#define DIR_BOUTON1 TRISBbits.TRISB0

#define POTAR PORTAbits.RA0
#define DIR_POTAR ANSELAbits.ANSA0

char offset = 1;
char String[2] = "XX";
char * DebugValue;

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
    
    //Configuration interrupt
    INTCONbits.GIE = 1; //enable all interrupts
    INTCONbits.PEIE = 1; //enable peripherals interrupts
    PIE1bits.RCIE = 1; //enable the USART receive interrupts

    //Init affichage
    SPI_InitializePins();
    LCD_InitializePins();
    SPI_Initialize();
    LCD_Initialize();
}

void init_pot(void){
    DIR_POTAR = 1;
    ANSELAbits.ANSA0 = 0; //Config en analogique
}

void init_adc(void) {
    ADCON0bits.ADON = 1;       // Active le module ADC
    ADCON1bits.ADCS = 0b00;    // Sélectionne la fréquence de conversion FRC (internal RC oscillator)(clock select) 
    ADCON1bits.ADPREF = 0b00;  // Sélectionne la référence VDD 0V
    ADCON1bits.ADNREF = 0;
    ADCON1bits.ADFM = 1;       // Résultat justifié à droite (format de restitution)
    ADCON0bits.CHS = 0;        // Potentiomètre est relié à la broche AN0/RA0
}

void __interrupt() isr(void) {
    if (PIE1bits.RCIE && PIR1bits.RCIF) {
        TX1REG = (char)((int)RC1REG + offset);
    }
}
/*
void affichage(void) {
    
    LCD_Clear();
    LCD_GoTo(0,0);
    
    if(){
        
    }else{
      LCD_WriteString();
    }
    
    
}
 */
char setOffset(void){
    ADCON0bits.ADGO = 1;                //Lance une conversion ADC
    while (ADCON0bits.ADGO){};          // Attend la fin de la conversion
    int potar = (ADRESH << 8) + ADRESL;
    sprintf(DebugValue,"%d",potar);
    return (35*potar)/1023;
}

void chiffrer(void){
    TX1REG = (char)((int)RC1REG + offset);
}

void dechiffrer(void){
    TX1REG = (char)((int)RC1REG - offset);
}

void charToRien(char nombre){
    char unite = (nombre%10)+48;
    char dizaines = (nombre/10)+48;
    String[0] = dizaines;
    String[1] = unite;    
}

void main(void) {
    /* Code d'initialisation */
    init();
    
    init_pot();
    init_adc();
    
    /* valeur_adc();
    offset = setOffset();        
        valeur_adc();
        offset = setOffset();
        charToRien(offset);*/
        LCD_Clear();
        LCD_GoTo(0,0);
        LCD_WriteString(String);
        LCD_Clear();
        
    while (1) {
        LCD_Clear();
        offset = setOffset();
        charToRien(offset);
        LCD_GoTo(0,0);
        LCD_WriteString(DebugValue);
        __delay_ms(250);
        
        if(BOUTON1 == 1) {
            chiffrer();
        }else{
            dechiffrer();
        }
    }
}