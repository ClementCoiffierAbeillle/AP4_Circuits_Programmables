/*
 * File: main.c
 * Author: clément.coiffier & jeremie.warconsin
 */

#include "configbits.h" // Bits de configuration
#include <xc.h>         // Definition des registres specifiques au uC
#define DELAY 1
#define _XTAL_FREQ 8000000
#include "../Ressources/lcd.h"
#include "../Ressources/spi.h"
#include "string.h"
#include "stdio.h"

#define BOUTON1 PORTBbits.RB0
#define DIR_BOUTON1 TRISBbits.TRISB0

#define POTAR PORTAbits.RA0
#define DIR_POTAR ANSELAbits.ANSA0

char offset = 1;
char String[3] = "XX";
int Chiffrement;

void init() {
    //Assignation RX/TX
    RC6PPSbits.RC6PPS = 0x14; //10100 p.154
    RXPPSbits.RXPPS = 0x17;   //10111 p.153

    //Configuration des pins RC6&RC7
    TRISCbits.TRISC6 = 0; //définie C6 en sortie
    TRISCbits.TRISC7 = 1; //définie C7 en entrée
    ANSELCbits.ANSC7 = 0; //numerique

    //Config bouton
    DIR_BOUTON1 = 1;      //Définie B0 en entrée
    ANSELBbits.ANSB0 = 0; //numérique

    //Configuration du Baudrate à 9600 bauds
    TX1STAbits.SYNC = 0;        //p.363
    TX1STAbits.BRGH = 0;        //low speed p.368
    BAUD1CONbits.BRG16 = 0;
    SP1BRGHbits.SP1BRGH = 0;    //high bits
    SP1BRGLbits.SP1BRGL = 0x0C; //low bits

    //Activation du module EUSART
    RC1STAbits.SPEN = 1; //serial port enable
    TX1STAbits.TXEN = 1; //transmit enable
    RC1STAbits.CREN = 1; //enables receiver  

    //Configuration interrupt
    INTCONbits.GIE = 1;  //enable all interrupts
    INTCONbits.PEIE = 1; //enable peripherals interrupts
    PIE1bits.RCIE = 1;   //enable the USART receive interrupts

    //Init affichage
    SPI_InitializePins();
    LCD_InitializePins();
    SPI_Initialize();
    LCD_Initialize();
}

void configurerInterruptBouton() {
    INTCONbits.INTE = 1; // Active l'interruption externe
    INTCONbits.INTF = 0; // Efface le drapeau d'interruption externe
    INTEDG = 0;          // Interruption sur front descendant
    INTCONbits.GIE = 1;  // Active les interruptions globales
}

void init_pot(void) {
    DIR_POTAR = 1;
    ANSELAbits.ANSA0 = 0; //Config en analogique
}

void init_adc(void) {
    ADCON0bits.ADON = 1;      // Active le module ADC
    ADCON1bits.ADCS = 0b00;   // Sélectionne la fréquence de conversion FRC (internal RC oscillator)(clock select) 
    ADCON1bits.ADPREF = 0b00; // Sélectionne la référence VDD 0V
    ADCON1bits.ADNREF = 0;
    ADCON1bits.ADFM = 1;      // Résultat justifié à droite (format de restitution)
    ADCON0bits.CHS = 0;       // Potentiomètre est relié à la broche AN0/RA0
}

void lcd() {
    LCD_Clear();    // Efface l'écran LCD
    LCD_GoTo(0, 0); // Place le curseur LCD à la position (0, 0)

    // Vérifie le mode de chiffrement
    if (Chiffrement == 1) {
       
        sprintf(String, "Chiffre : %d", offset);   // Formate la chaîne de caractères avec l'offset
        LCD_WriteString(String);                   // Affiche la chaîne de caractères sur l'écran LCD
    } else {
     
        sprintf(String, "Dechiffre : %d", offset); // Formate la chaîne de caractères avec l'offset
        LCD_WriteString(String);                   // Affiche la chaîne de caractères sur l'écran LCD
    }
}

void __interrupt() isr(void) {
    char caractere;

    while (PIR1bits.RCIF) {                                     // Tant que des caractères sont disponibles dans le buffer de réception
        caractere = RC1REG;                                     // Lecture du caractère reçu RC1REG

        if (caractere >= 'A' && RC1REG <= 'Z') {                // Si le caractère est une majuscule
            caractere += ('a' - 'A');                           // Convertir en minuscule
        }

        if (caractere >= '0' && caractere <= '9') {             // Si le caractère est un chiffre
            caractere = (char) cesar((int) caractere, offset);  // Appliquer le chiffrement César
        } else if (caractere >= 'a' && caractere <= 'z') {      // Si le caractère est une minuscule
            caractere = (char) cesar((int) caractere, offset);  // Appliquer le chiffrement César
        }

        TX1REG = caractere;                                     // Transmettre le caractère traité
    }

    if (INTF) {                                                 // Si c'est l'interruption externe
        INTF = 0;                                               // Efface le drapeau d'interruption externe

        if (Chiffrement == 1) {
            Chiffrement = 0;
        } else {
            Chiffrement = 1;
        }

        lcd();                                                 // Met à jour l'affichage LCD
    }
}


// Cette fonction lit la valeur du potentiomètre à l'aide du module ADC et retourne la valeur convertie en fonction de la tension appliquée au potentiomètre.
char setOffset(void) {
    ADCON0bits.ADGO = 1;                          // Lance une conversion ADC
    while (ADCON0bits.ADGO) {};                   // Attend la fin de la conversion
    unsigned int potar = (ADRESH << 8) + ADRESL;  // Concatène les valeurs de ADRESH et ADRESL pour obtenir la valeur ADC
    sprintf("%d", potar);                         // Affiche la valeur du potentiomètre (non utilisé ici)
    return (35 * potar) / 1023;                   // Convertit la valeur ADC en fonction de l'amplitude du potentiomètre
}

// Cette fonction chiffre le caractère reçu et l'envoie via la transmission série
void chiffrer(void) {
    TX1REG = (char) ((int) RC1REG + offset);      // Ajoute l'offset au caractère reçu et transmet le résultat
    Chiffrement = 1;                              // Définit le mode de chiffrement à 1 (chiffrement activé)
}

// Cette fonction déchiffre le caractère reçu et l'envoie via la transmission série
void dechiffrer(void) {
    TX1REG = (char) ((int) RC1REG - offset);      // Soustrait l'offset au caractère reçu et transmet le résultat
    Chiffrement = 0;                              // Définit le mode de chiffrement à 0 (déchiffrement activé)
}

// Cette fonction convertit un nombre en deux caractères ASCII représentant ses unités et dizaines
void charToRien(char nombre) {
    char unite = (nombre % 10) + 48;              // Calcule le chiffre des unités et le convertit en caractère ASCII
    char dizaines = (nombre / 10) + 48;           // Calcule le chiffre des dizaines et le convertit en caractère ASCII
    String[0] = dizaines;                         // Stocke le caractère représentant les dizaines dans la première position du tableau String
    String[1] = unite;                            // Stocke le caractère représentant les unités dans la deuxième position du tableau String
}

int cesar(int value, int offsetBis) {
    int renvoi;

    switch (Chiffrement) {                                // Sélectionne le mode de chiffrement ou de déchiffrement
        case 1:                                           // Si le mode est chiffrement
            if (value < 58) {                             // Si la valeur est un chiffre (code ASCII 48-57)
                if (value + offsetBis >= 58) {            // Si le décalage dépasse les chiffres
                    offsetBis = offsetBis - (58 - value); // Ajuste le décalage pour éviter de sortir des chiffres
                    renvoi = cesar(97, offsetBis);        // Applique récursivement le chiffrement avec le décalage ajusté et en utilisant 'a' (97) comme valeur de référence
                } else {
                    renvoi = value + offsetBis;           // Applique le décalage sans dépasser la plage des chiffres
                }
            } else {                                      // Si la valeur est une lettre minuscule (code ASCII 97-122)
                if (value + offsetBis >= 123) {           // Si le décalage dépasse les lettres minuscules
                    offsetBis = offsetBis - (123 - value);// Ajuste le décalage pour éviter de sortir des lettres minuscules
                    renvoi = cesar(48, offsetBis);        // Applique récursivement le chiffrement avec le décalage ajusté et en utilisant '0' (48) comme valeur de référence
                } else {
                    renvoi = value + offsetBis;           // Applique le décalage sans dépasser la plage des lettres minuscules
                }
            }
            break;
        default:                                          // Si le mode est déchiffrement
            if (value > 96) {                             // Si la valeur est une lettre minuscule (code ASCII 97-122)
                if (value - offsetBis <= 96) {            // Si le décalage dépasse les lettres minuscules
                    offsetBis = offsetBis - (value - 96); // Ajuste le décalage pour éviter de sortir des lettres minuscules
                    renvoi = cesar(57, offsetBis);        // Applique récursivement le déchiffrement avec le décalage ajusté et en utilisant '9' (57) comme valeur de référence
                } else {
                    renvoi = value - offsetBis;           // Applique le décalage sans dépasser la plage des lettres minuscules
                }
            } else {                                      // Si la valeur est un chiffre (code ASCII 48-57)
                if (value - offsetBis <= 47) {            // Si le décalage dépasse les chiffres
                    offsetBis = offsetBis - (value - 47); // Ajuste le décalage pour éviter de sortir des chiffres
                    renvoi = cesar(122, offsetBis);       // Applique récursivement le déchiffrement avec le décalage ajusté et en utilisant 'z' (122) comme valeur de référence
                } else {
                    renvoi = value - offsetBis;           // Applique le décalage sans dépasser la plage des chiffres
                }
            }
    }

    return renvoi;                                        // Retourne la valeur chiffrée ou déchiffrée
}

void main(void) {
    /* Code d'initialisation */
    init();
    init_pot();
    init_adc();
    LCD_Clear();
    LCD_GoTo(0, 0);
    //LCD_WriteString(String);
    __delay_ms(250);


    while (1) {
        LCD_Clear();
        offset = setOffset();
        charToRien(offset);
        LCD_GoTo(0, 0);
        //LCD_WriteString(String);
        lcd();
        __delay_ms(250);
        configurerInterruptBouton();
    }
}