/*
 * File: main.c
 * Author: cl�ment.coiffier & jeremie.warconsin
 * 
 * Cr�er un programme faisant varier contin�ment l?intensit� lumineuse de la diode D1 :
 * la diode D1 doit passer contin�ment de l?�tat �teint � allum�, puis de l?�tat allum� � �teint avec une p�riode de 2 s 
 */

#include "configbits.h" // Bits de configuration
#include <xc.h>         // D�finition des registres sp�cifiques au microcontr�leur

#define LED1 LATDbits.LATD0
#define DIR_LED1 TRISDbits.TRISD0

void init_timer2(void) {
    // Configuration du Timer2
    T2CONbits.T2CKPS = 0; // Pr�division par 16
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



