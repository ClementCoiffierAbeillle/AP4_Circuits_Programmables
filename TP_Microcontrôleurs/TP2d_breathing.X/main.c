/*
 * File: main.c
 * Author: clément.coiffier & jeremie.warconsin
 * 
 * Créer un programme faisant varier continûment l?intensité lumineuse de la diode D1 :
 * la diode D1 doit passer continûment de l?état éteint à allumé, puis de l?état allumé à éteint avec une période de 2 s 
 */

#include "configbits.h" // Bits de configuration
#include <xc.h>         // Définition des registres spécifiques au microcontrôleur

#define LED1 LATDbits.LATD0
#define DIR_LED1 TRISDbits.TRISD0

void init_timer2(void) {
    // Configuration du Timer2
    T2CONbits.T2CKPS = 0; // Prédivision par 16
    T2CONbits.T2OUTPS = 7; // Postdivision par 1
    PR2 = 255; // Valeur de rechargement pour obtenir 1ms
    T2CONbits.TMR2ON = 1; // Activation du Timer2
}

void init_leds(void) {
    DIR_LED1 = 0; LED1 = 0;
}

void init_pwm(void) {
    RD0PPSbits.RD0PPS = 0x0F; //Activer PWM module
    PWM4CONbits.PWM4EN = 1;
    PWM4DCH = 0;
    PWM4DCL = 0;    
    ANSELBbits.ANSB0 = 0;  
}

void main(void) {

    init_leds();
    init_timer2();
    init_pwm();
    
    while(1){
        for (int i = 0; i < 1024; i++) {
            PWM4DCH = i >> 2;
           PWM4DCL = i &3;
            while(PIR1bits.TMR2IF == 0);
            PIR1bits.TMR2IF = 0;
        }
        for (int i = 1024; i > 0; i--) {
            PWM4DCH = i >> 2;
            PWM4DCL = i &3;
            while(PIR1bits.TMR2IF == 0);
            PIR1bits.TMR2IF = 0; 
        }
    }

}



