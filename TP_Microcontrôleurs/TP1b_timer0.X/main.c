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
    int iterations = 125; //250 en tout pour les deux états
    TMR0 = 131;

    for (int i = 0; i < iterations; i++) {
        while (!INTCONbits.TMR0IF) {
            // Attendre que l'interruption du Timer 0 se produise
        }
        INTCONbits.TMR0IF = 0;
    }
}

void init_timer0(void) {
    OPTION_REGbits.TMR0CS = 0; // Source d'horloge interne pour le Timer 0
    OPTION_REGbits.PSA = 0;    // Activation du prédiviseur pour le Timer 0
    OPTION_REGbits.PS = 0b101; // Réglage du prédiviseur à 1:256 (pour une bonne précision)
}

void main(void) {
    /* Code d'initialisation */
    init_leds();
    init_timer0();

    while (1) {
        /* Code à exécuter dans une boucle infinie */
        LED1 = 1;
        LED2 = 1;
        LED3 = 1;
        LED4 = 1;
        LED5 = 0;
        LED6 = 0;
        LED7 = 0;
        LED8 = 0;
        delay_parfait();
        LED1 = 0;
        LED2 = 0;
        LED3 = 0;
        LED4 = 0;
        LED5 = 1;
        LED6 = 1;
        LED7 = 1;
        LED8 = 1;
        delay_parfait();
    }
}
