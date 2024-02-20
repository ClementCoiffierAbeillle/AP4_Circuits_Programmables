/*
 * File: main.c
 * Author: clément.coiffier & jeremie.warconsin
 */

#include "configbits.h" // Inclut le fichier de configuration des bits
#include <xc.h>         // Inclut les définitions des registres spécifiques au microcontrôleur
#define DELAY 1
#define _XTAL_FREQ 8000000

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

volatile unsigned char compteurTimerOverflow = 0;
volatile unsigned char nombreLeds = 4;
volatile unsigned char compteur = 0;

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

// Fonction pour initialiser le Timer 2
void init_timer(void) {
    // Configuration du Timer2
    T2CONbits.T2CKPS = 2;      // Prédivision par 16
    T2CONbits.T2OUTPS = 0;     // Postdivision par 1
    PR2 = 124;                 // Valeur de rechargement pour obtenir 1ms
    T2CONbits.TMR2ON = 1;      // Activation du Timer2
    TMR2 = 0;                  // Réinitialisation du compteur du Timer2
}

// Fonction pour initialiser les interruptions
void init_interrupt(void) {
    INTCONbits.GIE = 1;  // Activation des interruptions globales
    INTCONbits.PEIE = 1; // Activation des interruptions périphériques
    PIE1bits.TMR2IE = 1; // Activation de l'interruption du Timer2
    PIR1bits.TMR2IF = 1; // Réinitialisation du drapeau d'interruption du Timer2
}

// Fonction pour gérer la temporisation en millisecondes
void delay_ms(unsigned int milliseconds) {
    for (unsigned int i = 0; i < milliseconds; i++) {
        __delay_ms(1); // Utilisation de la fonction de délai fournie par le compilateur
    }
}

// Fonction d'interruption
void __interrupt() isr(void) {
    if (TMR2IE && TMR2IF) { // Vérifie si l'interruption du Timer2 est activée et si le drapeau d'interruption du Timer2 est levé
        TMR2IF = 0; // Réinitialisation du drapeau d'interruption du Timer2

        compteurTimerOverflow++; // Incrémente le compteur d'overflow du Timer2

        if (compteurTimerOverflow == 125) { // Si 125 overflows (1 seconde environ)
            // Actions à effectuer toutes les 125 overflows du Timer2

            switch (compteur % nombreLeds) { // Change de LED toutes les 125*4 overflows (4 secondes)
                case 0:
                    LED1 = 1; // Allume la LED1
                    LED4 = 0; // Éteint la LED4
                    compteur++; // Incrémente le compteur
                    break;
                case 1:
                    LED2 = 1;
                    LED1 = 0;
                    compteur++;
                    break;
                case 2:
                    LED3 = 1;
                    LED2 = 0;
                    compteur++;
                    break;
                case 3:
                    LED4 = 1;
                    LED3 = 0;
                    compteur = 0; // Remet le compteur à zéro
                    break;
            }

            compteurTimerOverflow = 0; // Réinitialisation du compteur d'overflows

        }
    }
}

void main(void) {
    /* Code d'initialisation */
    init_leds(); // Initialise les LEDs
    init_timer(); // Initialise le Timer 2
    init_interrupt(); // Initialise les interruptions

    while (1) {
        /* Code à exécuter dans une boucle infinie */
        // Cycle d'allumage des LEDs en séquence
        LED8 = 1; // Allume LED8
        LED5 = 0; // Éteint LED5
        delay_ms(1000); // Attente de 1 seconde

        LED7 = 1;
        LED8 = 0;
        delay_ms(1000);

        LED6 = 1;
        LED7 = 0;
        delay_ms(1000);

        LED5 = 1;
        LED6 = 0;
        delay_ms(1000);
    }
}
