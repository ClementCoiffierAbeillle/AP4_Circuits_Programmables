/*
 * File: main.c
 * Author: clément.coiffier & jeremie.warconsin
 */

#include "configbits.h" // Bits de configuration
#include <xc.h>         // Définition des registres spécifiques au microcontrôleur
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

#define BOUTON PORTAbits.RA5
#define DIR_BOUTON TRISAbits.TRISA5

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

void init_timer2(void) {
    // Configuration du Timer2
    T2CONbits.T2CKPS = 2;  // Prédivision par 16
    T2CONbits.T2OUTPS = 0; // Postdivision par 1
    PR2 = 124;             // Valeur de rechargement pour obtenir 1ms
    T2CONbits.TMR2ON = 1;  // Activation du Timer2
    TMR2 = 0;              // Réinitialisation du compteur du Timer2
}

void init_interrupt(void) {
    INTCONbits.GIE = 1;  // Activation des interruptions globales
    INTCONbits.PEIE = 1; // Activation des interruptions périphériques
    PIE1bits.TMR2IE = 1; // Activation de l'interruption du Timer2
    PIR1bits.TMR2IF = 1;

    INTCONbits.IOCIE = 1; // Interrupt on change
    IOCANbits.IOCAN5 = 1; // Interrupt on port A5


}

void init_bouton(void) {
    //Config bouton
    DIR_BOUTON = 1;       //Définie Bouton en entrée
    ANSELAbits.ANSA5 = 0; //numérique
}

// Fonction pour gérer la temporisation

void delay_ms(unsigned int milliseconds) {
    for (unsigned int i = 0; i < milliseconds; i++) {
        __delay_ms(1);                        // Utilisation de la fonction de délai fournie par le compilateur
    }
}

void __interrupt() isr(void) {


    if (IOCAFbits.IOCAF5 == 1) {              // Vérifie si l'interruption est due à un changement sur le port A5
        IOCAFbits.IOCAF5 = 0;                 // Efface le drapeau d'interruption pour le port A5 pour indiquer que l'interruption a été traitée
        T2CONbits.TMR2ON = !T2CONbits.TMR2ON; // Inverse l'état du Timer2 (l'active s'il est désactivé, et le désactive s'il est activé)
    }

    if (IOCBFbits.IOCBF0 == 1) {              // Vérifie si l'interruption est due au bouton B0
        IOCBFbits.IOCBF0 = 0;                 // Efface le drapeau d'interruption
        T2CONbits.TMR2ON = ~T2CONbits.TMR2ON; // Inverse l'état du Timer2
    }

    if (TMR2IE && TMR2IF) {                   // Code à exécuter lors de l'interruption du Timer2
        TMR2IF = 0;                           // Réinitialisation du drapeau d'interruption

        compteurTimerOverflow++;

        if (compteurTimerOverflow == 125) {   // Action à effectuer tous les 125 overflows du Timer2

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

            compteurTimerOverflow = 0;      // Réinitialisation du compteur d'overflows
        }
    }
}

void main(void) {
    /* Code d'initialisation */
    init_leds();
    init_timer2();
    init_interrupt();
    init_bouton();
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
