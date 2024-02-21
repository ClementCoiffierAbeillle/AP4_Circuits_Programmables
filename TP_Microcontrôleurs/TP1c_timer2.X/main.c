/*
 * File: main.c
 * Author: cl�ment.coiffier & jeremie.warconsin
 * Dans cet exercice, nous changerons le motif affich� par les LEDs (de mani�re � les piloter une par une).
 * En plus de cela, nous utiliserons une source diff�rente de temporisation.
 */

#include "configbits.h" // Inclut le fichier de configuration des bits
#include <xc.h>         // Inclut les d�finitions des registres sp�cifiques au microcontr�leur

// D�finition des broches des LEDs et de leur direction
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
    DIR_LED1 = 0; LED1 = 0; // Configure la broche LED1 en sortie et l'�teint
    DIR_LED2 = 0; LED2 = 0; // Configure la broche LED2 en sortie et l'�teint
    DIR_LED3 = 0; LED3 = 0; // Configure la broche LED3 en sortie et l'�teint
    DIR_LED4 = 0; LED4 = 0; // Configure la broche LED4 en sortie et l'�teint
    DIR_LED5 = 0; LED5 = 0; // Configure la broche LED5 en sortie et l'�teint
    DIR_LED6 = 0; LED6 = 0; // Configure la broche LED6 en sortie et l'�teint
    DIR_LED7 = 0; LED7 = 0; // Configure la broche LED7 en sortie et l'�teint
    DIR_LED8 = 0; LED8 = 0; // Configure la broche LED8 en sortie et l'�teint
}

// Fonction pour cr�er un retard pr�cis
void delay_parfait(void) {
    int iterations = 125; // Nombre d'it�rations pour obtenir un d�lai de 125 ms (0.125 secondes)
    for (int i = 0; i < iterations; i++) {
        while (!PIR1bits.TMR2IF) {
            // Attend que l'interruption du Timer 2 se produise
        }
        PIR1bits.TMR2IF = 0; // R�initialise le drapeau d'interruption du Timer 2
    }
}

// Fonction pour initialiser le Timer 2
void init_timer2(void) {
    // Configuration du Timer 2
    T2CONbits.T2CKPS = 2; // Pr�division par 16
    T2CONbits.T2OUTPS = 0; // Postdivision par 1
    PR2 = 124; // Valeur de rechargement pour obtenir un d�bordement toutes les 1 millisecondes
    T2CONbits.TMR2ON = 1; // Activation du Timer 2
    TMR2 = 0; // R�initialisation du compteur du Timer 2
}

// Fonction principale
void main(void) {
    /* Code d'initialisation */
    init_leds(); // Initialise les LEDs
    init_timer2(); // Initialise le Timer 2

    // Attend un petit moment pour permettre aux composants de se stabiliser
    delay_parfait();
    
    while (1) {
        /* Code � ex�cuter dans une boucle infinie */
        // Allume les LEDs une � une avec un d�lai de 125 ms entre chaque allumage
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
