/*
 * File: main.c
 * Author: cl�ment.coiffier & jeremie.warconsin
 */

#include "configbits.h" // Bits de configuration
#include <xc.h>         // D�finition des registres sp�cifiques au microcontr�leur
#define DELAY 1
#define _XTAL_FREQ 8000000

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

void init_leds(void) {
    DIR_LED1 = 0;
    LED1 = 0;
    DIR_LED2 = 0;
    LED2 = 0;
    DIR_LED3 = 0;
    LED3 = 0;
    DIR_LED4 = 0;
    LED4 = 0;
    DIR_LED5 = 0;
    LED5 = 0;
    DIR_LED6 = 0;
    LED6 = 0;
    DIR_LED7 = 0;
    LED7 = 0;
    DIR_LED8 = 0;
    LED8 = 0;
}

void init_timer(void) {
    // Configuration du Timer2
    T2CONbits.T2CKPS = 2;      // Pr�division par 16
    T2CONbits.T2OUTPS = 0;     // Postdivision par 1
    PR2 = 124;                 // Valeur de rechargement pour obtenir 1ms
    T2CONbits.TMR2ON = 1;      // Activation du Timer2
    TMR2 = 0;                  // R�initialisation du compteur du Timer2
}

void init_interrupt(void) {
    INTCONbits.GIE = 1;  // Activation des interruptions globales
    INTCONbits.PEIE = 1; // Activation des interruptions p�riph�riques
    PIE1bits.TMR2IE = 1; // Activation de l'interruption du Timer2
    PIR1bits.TMR2IF = 1;
}

// Fonction pour g�rer la temporisation
void delay_ms(unsigned int milliseconds) {
    for (unsigned int i = 0; i < milliseconds; i++) {
        __delay_ms(1);                      // Utilisation de la fonction de d�lai fournie par le compilateur
    }
}

void __interrupt() isr(void) {

    if (TMR2IE && TMR2IF) {                 // Code � ex�cuter lors de l'interruption du Timer2
        TMR2IF = 0;                         // R�initialisation du drapeau d'interruption

        compteurTimerOverflow++;

        if (compteurTimerOverflow == 125) { // Action � effectuer tous les 125 overflows du Timer2

            switch (compteur % nombreLeds) {
                case 0:
                    LED1 = 1;
                    LED4 = 0;
                    compteur++;
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
                    compteur = 0;
                    break;
            }

            compteurTimerOverflow = 0; // R�initialisation du compteur d'overflows

        }
    }
}

void main(void) {
    /* Code d'initialisation */
    init_leds();
    init_timer();
    init_interrupt();

    while (1) {
        
    LED8 = 1;
    LED5 = 0;
    delay_ms(1000);

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
  