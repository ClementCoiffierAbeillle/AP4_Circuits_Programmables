/*
 * File: main.c
 * Author: cl�ment.coiffier & jeremie.warconsin
 */

#include "configbits.h" // Inclut le fichier de configuration des bits
#include <xc.h>         // Inclut les d�finitions des registres sp�cifiques au microcontr�leur
#define DELAY 1
#define _XTAL_FREQ 8000000

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

volatile unsigned char compteurTimerOverflow = 0;
volatile unsigned char nombreLeds = 4;
volatile unsigned char compteur = 0;

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

// Fonction pour initialiser le Timer 2
void init_timer(void) {
    // Configuration du Timer2
    T2CONbits.T2CKPS = 2;      // Pr�division par 16
    T2CONbits.T2OUTPS = 0;     // Postdivision par 1
    PR2 = 124;                 // Valeur de rechargement pour obtenir 1ms
    T2CONbits.TMR2ON = 1;      // Activation du Timer2
    TMR2 = 0;                  // R�initialisation du compteur du Timer2
}

// Fonction pour initialiser les interruptions
void init_interrupt(void) {
    INTCONbits.GIE = 1;  // Activation des interruptions globales
    INTCONbits.PEIE = 1; // Activation des interruptions p�riph�riques
    PIE1bits.TMR2IE = 1; // Activation de l'interruption du Timer2
    PIR1bits.TMR2IF = 1; // R�initialisation du drapeau d'interruption du Timer2
}

// Fonction pour g�rer la temporisation en millisecondes
void delay_ms(unsigned int milliseconds) {
    for (unsigned int i = 0; i < milliseconds; i++) {
        __delay_ms(1); // Utilisation de la fonction de d�lai fournie par le compilateur
    }
}

// Fonction d'interruption
void __interrupt() isr(void) {
    if (TMR2IE && TMR2IF) { // V�rifie si l'interruption du Timer2 est activ�e et si le drapeau d'interruption du Timer2 est lev�
        TMR2IF = 0; // R�initialisation du drapeau d'interruption du Timer2

        compteurTimerOverflow++; // Incr�mente le compteur d'overflow du Timer2

        if (compteurTimerOverflow == 125) { // Si 125 overflows (1 seconde environ)
            // Actions � effectuer toutes les 125 overflows du Timer2

            switch (compteur % nombreLeds) { // Change de LED toutes les 125*4 overflows (4 secondes)
                case 0:
                    LED1 = 1; // Allume la LED1
                    LED4 = 0; // �teint la LED4
                    compteur++; // Incr�mente le compteur
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
                    compteur = 0; // Remet le compteur � z�ro
                    break;
            }

            compteurTimerOverflow = 0; // R�initialisation du compteur d'overflows

        }
    }
}

void main(void) {
    /* Code d'initialisation */
    init_leds(); // Initialise les LEDs
    init_timer(); // Initialise le Timer 2
    init_interrupt(); // Initialise les interruptions

    while (1) {
        /* Code � ex�cuter dans une boucle infinie */
        // Cycle d'allumage des LEDs en s�quence
        LED8 = 1; // Allume LED8
        LED5 = 0; // �teint LED5
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
