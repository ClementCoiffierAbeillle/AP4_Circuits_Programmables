/*
 * File: main.c
 * Author: cl�ment.coiffier & jeremie.warconsin
 *
 * Dans ce premier exercice, il s?agit de r�aliser le � Hello, World ! � du microcontr�leur : 
 * faire clignoter des LEDs, ou r�aliser le programme � blink �. 
 */

#include "configbits.h" // Bits de configuration
#include <xc.h>         // D�finition des registres sp�cifiques au uC

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

// D�finition d'une constante pour la dur�e de retard
const long int delay_cycles = 10000;

// Fonction pour initialiser les LEDs
void init_leds(void){
    // Configuration des directions des broches des LEDs en sortie
    DIR_LED1 = 0; LED1 = 0;
    DIR_LED2 = 0; LED2 = 0;
    DIR_LED3 = 0; LED3 = 0;
    DIR_LED4 = 0; LED4 = 0;
    DIR_LED5 = 0; LED5 = 0;
    DIR_LED6 = 0; LED6 = 0;
    DIR_LED7 = 0; LED7 = 0;
    DIR_LED8 = 0; LED8 = 0;
}

// Fonction pour un retard approximatif
void delay_approx(void){
    for(long int i=0; i<delay_cycles; i++){}
}

// Fonction principale
void main(void) {
    /* Code d'initialisation */
    init_leds();
    
    while(1){
        /* Code � ex�cuter dans une boucle infinie */
        // Allumer les LEDs 1 � 4 et �teindre les LEDs 5 � 8
        LED1 = 1;
        LED2 = 1;
        LED3 = 1;
        LED4 = 1;
        LED5 = 0;
        LED6 = 0;
        LED7 = 0;
        LED8 = 0;
        // Retard approximatif
        delay_approx();
        // �teindre les LEDs 1 � 4 et allumer les LEDs 5 � 8
        LED1 = 0;
        LED2 = 0;
        LED3 = 0;
        LED4 = 0;
        LED5 = 1;
        LED6 = 1;
        LED7 = 1;
        LED8 = 1;
        // Retard approximatif
        delay_approx(); 
    }
}
