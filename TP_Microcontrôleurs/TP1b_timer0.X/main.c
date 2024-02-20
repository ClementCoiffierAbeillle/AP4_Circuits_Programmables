/*
 * File: main.c
 * Author: clément.coiffier & jeremie.warconsin
 */

#include "configbits.h" // Fichier de configuration des bits
#include <xc.h>         // Inclusion des registres spécifiques au microcontrôleur

// Définition des broches des LEDs et de leur direction
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

// Fonction pour initialiser les LEDs
void init_leds(void) {
    DIR_LED1 = 0; LED1 = 0; // Configure la broche LED1 en sortie et l'éteint
    DIR_LED2 = 0; LED2 = 0; // Configure la broche LED2 en sortie et l'éteint
    DIR_LED3 = 0; LED3 = 0; // Configure la broche LED3 en sortie et l'éteint
    DIR_LED4 = 0; LED4 = 0; // Configure la broche LED4 en sortie et l'éteint
    DIR_LED5 = 0; LED5 = 0; // Configure la broche LED5 en sortie et l'éteint
    DIR_LED6 = 0; LED6 = 0; // Configure la broche LED6 en sortie et l'éteint
    DIR_LED7 = 0; LED7 = 0; // Configure la broche LED7 en sortie et l'éteint
    DIR_LED8 = 0; LED8 = 0; // Configure la broche LED8 en sortie et l'éteint
}

// Fonction pour créer un retard précis
void delay_parfait(void) {
    int iterations = 125; // Nombre d'itérations pour obtenir un délai d'une seconde
    
    // Configuration du Timer 0 pour le délai
    TMR0 = 131; // Initialisation du Timer 0 pour un délai d'une seconde
    
    // Boucle pour attendre le délai
    for (int i = 0; i < iterations; i++) {
        while (!INTCONbits.TMR0IF) {
            // Attend que l'interruption du Timer 0 se produise
        }
        INTCONbits.TMR0IF = 0; // Réinitialisation du drapeau d'interruption
    }
}

// Fonction pour initialiser le Timer 0
void init_timer0(void) {
    OPTION_REGbits.TMR0CS = 0; // Source d'horloge interne pour le Timer 0
    OPTION_REGbits.PSA = 0;    // Activation du prédiviseur pour le Timer 0
    OPTION_REGbits.PS = 0b101; // Réglage du prédiviseur à 1:256 (pour une bonne précision)
}

// Fonction principale
void main(void) {
    /* Code d'initialisation */
    init_leds(); // Initialise les LEDs
    init_timer0(); // Initialise le Timer 0

    while (1) {
        /* Code à exécuter dans une boucle infinie */
        // Allume les LEDs 1 à 4 et éteint les LEDs 5 à 8
        LED1 = 1;
        LED2 = 1;
        LED3 = 1;
        LED4 = 1;
        LED5 = 0;
        LED6 = 0;
        LED7 = 0;
        LED8 = 0;
        delay_parfait(); // Attend un délai
        // Éteint les LEDs 1 à 4 et allume les LEDs 5 à 8
        LED1 = 0;
        LED2 = 0;
        LED3 = 0;
        LED4 = 0;
        LED5 = 1;
        LED6 = 1;
        LED7 = 1;
        LED8 = 1;
        delay_parfait(); // Attend un délai
    }
}
