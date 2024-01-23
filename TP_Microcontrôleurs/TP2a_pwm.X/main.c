#include "configbits.h" // Bits de configuration
#include <xc.h>         // Définition des registres spécifiques au microcontrôleur

#define LED1 LATDbits.LATD0
#define BOUTON1 PORTBbits.RB0

#define DIR_LED1 TRISDbits.TRISD0
#define DIR_BOUTON1 TRISBbits.TRISB0



void init_leds(void) {
    DIR_LED1 = 0; LED1 = 0;
}

void init_rb0(void) {
    // Configuration du Bouton S1T
    DIR_BOUTON1 = 1;  
    ANSELBbits.ANSB0 = 0; //Config en analogique
}

void init_pwm(void) {
    T2CONbits.T2CKPS = 0b11;//Config prescaler à 64 du timer2
    T2CONbits.TMR2ON = 1;//Activation du timer2
    PR2 = 255; //Période de la PWM
    RD0PPS = 0x0F;
    PWM4CONbits.PWM4EN = 1;
    PWM4DCHbits.PWM4DCH = 0x66;
    PWM4DCLbits.PWM4DCL = 0x00;
    
}

void main(void) {
    /* Code d'initialisation */
    init_leds();
    init_rb0();
    init_pwm();
    
    while (1) {
        if (BOUTON1 == 0 ){
            PWM4DCHbits.PWM4DCH = 0xE6; //Rapport cyclique de 90%
            PWM4DCLbits.PWM4DCL = 0x80;
        }else{
            PWM4DCHbits.PWM4DCH = 0x19; //Rapport cyclique de 10%
            PWM4DCLbits.PWM4DCL = 0x80;
        }
    }
    return;
}

