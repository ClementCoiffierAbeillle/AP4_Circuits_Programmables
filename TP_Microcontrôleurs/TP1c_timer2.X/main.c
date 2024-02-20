/*
 * File: main.c
 * Author: clément.coiffier & jeremie.warconsin
 */

#include "configbits.h" // Bits de configuration
#include <xc.h>         // Définition des registres spécifiques au microcontrôleur

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

void delay_parfait(void) {
    // Calcul du nombre d'itérations pour obtenir un délai d'une seconde
    int iterations = 125; //Pour arriver à 125ms
    for (int i = 0; i < iterations; i++) {
        while (!PIR1bits.TMR2IF) {
            // Attendre que l'interruption du Timer 2 se produise
        }
        PIR1bits.TMR2IF = 0;
    }
}

void init_timer2(void) {
    // Configuration du Timer2
    T2CONbits.T2CKPS = 2; // Prédivision par 16
    T2CONbits.T2OUTPS = 0; // Postdivision par 1
    PR2 = 124; // Valeur de rechargement pour obtenir 1ms
    T2CONbits.TMR2ON = 1; // Activation du Timer2
    TMR2 = 0; // Réinitialisation du compteur du Timer2
}


void main(void) {
    /* Code d'initialisation */
    init_leds();
    init_timer2();
    delay_parfait();
    
    while (1) {
        /* Code à exécuter dans une boucle infinie */
        LED1 = 1;
        LED8 = 0;
        delay_parfait();
        LED2 = 1;
        LED1 = 0;
        delay_parfait();
        LED3 = 1;
        LED2 = 0;
        delay_parfait();
        LED4 = 1;
        LED3 = 0;
        delay_parfait();
        LED5 = 1;
        LED4 = 0;
        delay_parfait();
        LED6 = 1;
        LED5 = 0;
        delay_parfait();
        LED7 = 1;
        LED6 = 0;
        delay_parfait();
        LED8 = 1;
        LED7 = 0;
        delay_parfait();
    }
}
