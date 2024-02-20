/*
 * File: main.c
 * Author: cl�ment.coiffier & jeremie.warconsin
 */

#include "configbits.h" // Bits de configuration
#include <xc.h>         // D�finition des registres sp�cifiques au microcontr�leur

#define LED1 LATDbits.LATD0
#define LED2 LATDbits.LATD1
#define LED3 LATDbits.LATD2
#define LED4 LATDbits.LATD3
#define LED5 LATBbits.LATB0
#define LED6 LATBbits.LATB1
#define LED7 LATBbits.LATB2
#define LED8 LATBbits.LATB3

#define DIR_LED1 TRISDbits.TRISD0
#define DIR_LED2 TRISDbits.TRISD1
#define DIR_LED3 TRISDbits.TRISD2
#define DIR_LED4 TRISDbits.TRISD3
#define DIR_LED5 TRISBbits.TRISB0
#define DIR_LED6 TRISBbits.TRISB1
#define DIR_LED7 TRISBbits.TRISB2
#define DIR_LED8 TRISBbits.TRISB3

#define BOUTON1 PORTBbits.RB0
#define DIR_BOUTON1 TRISBbits.TRISB0

#define POTAR PORTAbits.RA0
#define DIR_POTAR ANSELAbits.ANSA0

void init_leds(void) {
    DIR_LED1 = 0; LED1 = 0;
    DIR_LED2 = 0; LED2 = 0;
    DIR_LED3 = 0; LED3 = 0;
    DIR_LED4 = 0; LED4 = 0;
    DIR_LED5 = 0; LED5 = 0;
    DIR_LED6 = 0; LED6 = 0;
    DIR_LED7 = 0; LED7 = 0;
    DIR_LED8 = 0; LED8 = 0;
}

void init_pot(void){
    DIR_POTAR = 1;
    ANSELAbits.ANSA0 = 0; //Config en analogique
}

void init_adc(void) {
    ADCON0bits.ADON = 1;       // Active le module ADC
    ADCON1bits.ADCS = 0b00;    // S�lectionne la fr�quence de conversion FRC (internal RC oscillator)(clock select) 
    ADCON1bits.ADPREF = 0b00;  // S�lectionne la r�f�rence VDD 0V
    ADCON1bits.ADNREF = 0;
    ADCON1bits.ADFM = 1;       // R�sultat justifi� � droite (format de restitution)
    ADCON0bits.CHS = 0;        // Potentiom�tre est reli� � la broche AN0/RA0
}


int valeur_adc(void) {
    ADCON0bits.ADGO = 1;                //Lance une conversion ADC
    while (ADCON0bits.ADGO){};          // Attend la fin de la conversion
    return ((ADRESH << 8) + ADRESL);    //Retourne la valeur ADC lue (d�calage pour )
}

void main(void) {
    /* Code d'initialisation */
    init_leds();
    init_pot();
    init_adc();
   
    
    while (1) {
    int x = valeur_adc();
        if (x < 128) {
        LED1 = 1;
        LED2 = 0;
        LED3 = 0;
        LED4 = 0;
        LED5 = 0; 
        LED6 = 0;
        LED7 = 0;
        LED8 = 0;
        
    } else if (x < 256) {
        LED2 = 1;
        LED1 = 0;
        LED3 = 0;
        LED4 = 0;
        LED5 = 0; 
        LED6 = 0;
        LED7 = 0;
        LED8 = 0;
        
    } else if (x < 384) {
        LED3 = 1;
        LED2 = 0;
        LED1 = 0;
        LED4 = 0;
        LED5 = 0; 
        LED6 = 0;
        LED7 = 0;
        LED8 = 0;
        
    } else if (x < 512) {
        LED4 = 1;
        LED2 = 0;
        LED3 = 0;
        LED1 = 0;
        LED5 = 0; 
        LED6 = 0;
        LED7 = 0;
        LED8 = 0;
        
    } else if (x < 640) {
        LED5 = 1;
        LED2 = 0;
        LED3 = 0;
        LED4 = 0;
        LED1 = 0; 
        LED6 = 0;
        LED7 = 0;
        LED8 = 0;
        
    } else if (x < 768) {
        LED6 = 1;
        LED2 = 0;
        LED3 = 0;
        LED4 = 0;
        LED5 = 0; 
        LED1 = 0;
        LED7 = 0;
        LED8 = 0;
        
    } else if (x < 896) {
        LED7 = 1;
        LED2 = 0;
        LED3 = 0;
        LED4 = 0;
        LED5 = 0; 
        LED6 = 0;
        LED1 = 0;
        LED8 = 0;
        
    } else if (x < 1024) {
        LED8 = 1;
        LED2 = 0;
        LED3 = 0;
        LED4 = 0;
        LED5 = 0; 
        LED6 = 0;
        LED7 = 0;
        LED1 = 0;
    }
    
}

}

