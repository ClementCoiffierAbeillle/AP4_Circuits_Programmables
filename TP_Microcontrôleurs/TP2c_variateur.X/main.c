/*
 * File: main.c
 * Author: clément.coiffier & jeremie.warconsin
 */

#include "configbits.h" // Bits de configuration
#include <xc.h>         // Définition des registres spécifiques au microcontrôleur

#define LED1 LATDbits.LATD0

#define DIR_LED1 TRISDbits.TRISD0

#define BOUTON1 PORTBbits.RB0
#define DIR_BOUTON1 TRISBbits.TRISB0

#define POTAR PORTAbits.RA0
#define DIR_POTAR ANSELAbits.ANSA0

void init_leds(void) {
    DIR_LED1 = 0; LED1 = 0;
}

void init_pot(void){
    DIR_POTAR = 1;
    ANSELAbits.ANSA0 = 0; //Config en analogique
}

void init_adc(void) {
    ADCON0bits.ADON = 1;       // Active le module ADC
    ADCON1bits.ADCS = 0;    // Sélectionne la fréquence de conversion FRC (internal RC oscillator)(clock select) 
    ADCON1bits.ADPREF = 0;  // Sélectionne la référence VDD 0V
    ADCON1bits.ADNREF = 0;     // Sélectionne VREF 5V
    ADCON1bits.ADFM = 0;       // Résultat justifié à gauche (format de restitution)
    ADCON0bits.CHS = 0;        // Potentiomètre est relié à la broche AN0/RA0
}


void update_ADC(void) {
    ADCON0bits.ADGO = 1;                //Lance une conversion ADC
    while (ADCON0bits.ADGO){};          // Attend la fin de la conversion
    
}

void init_pwm(void) {
    T2CONbits.T2CKPS = 0b11;//Config prescaler à 64 du timer2
    T2CONbits.TMR2ON = 1;   //Activation du timer2
    PR2 = 255;              //Période de la PWM
    RD0PPS = 0x0F;
    PWM4CONbits.PWM4EN = 1;
    PWM4DCHbits.PWM4DCH = 0x66;
    PWM4DCLbits.PWM4DCL = 0x00;    

    
}

void main(void) {
    /* Code d'initialisation */
    init_leds();
    init_pot();
    init_adc();
    init_pwm();
    LED1 = 1;
    
    while (1) {
    update_ADC();
    PWM4DCHbits.PWM4DCH = ADRESH;
    PWM4DCLbits.PWM4DCL = ADRESL; 
     
}

}

