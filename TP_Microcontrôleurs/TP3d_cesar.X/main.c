/*
 * File: main.c
 * Author: cl�ment.coiffier & jeremie.warconsin
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
    TRISCbits.TRISC6 = 0; //d�finie C6 en sortie
    TRISCbits.TRISC7 = 1; //d�finie C7 en entr�e
    ANSELCbits.ANSC7 = 0; //numerique

    //Config bouton
    DIR_BOUTON1 = 1;      //D�finie B0 en entr�e
    ANSELBbits.ANSB0 = 0; //num�rique

    //Configuration du Baudrate � 9600 bauds
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
    ADCON1bits.ADCS = 0b00;   // S�lectionne la fr�quence de conversion FRC (internal RC oscillator)(clock select) 
    ADCON1bits.ADPREF = 0b00; // S�lectionne la r�f�rence VDD 0V
    ADCON1bits.ADNREF = 0;
    ADCON1bits.ADFM = 1;      // R�sultat justifi� � droite (format de restitution)
    ADCON0bits.CHS = 0;       // Potentiom�tre est reli� � la broche AN0/RA0
}

void lcd() {
    LCD_Clear();    // Efface l'�cran LCD
    LCD_GoTo(0, 0); // Place le curseur LCD � la position (0, 0)

    // V�rifie le mode de chiffrement
    if (Chiffrement == 1) {
       
        sprintf(String, "Chiffre : %d", offset);   // Formate la cha�ne de caract�res avec l'offset
        LCD_WriteString(String);                   // Affiche la cha�ne de caract�res sur l'�cran LCD
    } else {
     
        sprintf(String, "Dechiffre : %d", offset); // Formate la cha�ne de caract�res avec l'offset
        LCD_WriteString(String);                   // Affiche la cha�ne de caract�res sur l'�cran LCD
    }
}

void __interrupt() isr(void) {
    char caractere;

    while (PIR1bits.RCIF) {                                     // Tant que des caract�res sont disponibles dans le buffer de r�ception
        caractere = RC1REG;                                     // Lecture du caract�re re�u RC1REG

        if (caractere >= 'A' && RC1REG <= 'Z') {                // Si le caract�re est une majuscule
            caractere += ('a' - 'A');                           // Convertir en minuscule
        }

        if (caractere >= '0' && caractere <= '9') {             // Si le caract�re est un chiffre
            caractere = (char) cesar((int) caractere, offset);  // Appliquer le chiffrement C�sar
        } else if (caractere >= 'a' && caractere <= 'z') {      // Si le caract�re est une minuscule
            caractere = (char) cesar((int) caractere, offset);  // Appliquer le chiffrement C�sar
        }

        TX1REG = caractere;                                     // Transmettre le caract�re trait�
    }

    if (INTF) {                                                 // Si c'est l'interruption externe
        INTF = 0;                                               // Efface le drapeau d'interruption externe

        if (Chiffrement == 1) {
            Chiffrement = 0;
        } else {
            Chiffrement = 1;
        }

        lcd();                                                 // Met � jour l'affichage LCD
    }
}


// Cette fonction lit la valeur du potentiom�tre � l'aide du module ADC et retourne la valeur convertie en fonction de la tension appliqu�e au potentiom�tre.
char setOffset(void) {
    ADCON0bits.ADGO = 1;                          // Lance une conversion ADC
    while (ADCON0bits.ADGO) {};                   // Attend la fin de la conversion
    unsigned int potar = (ADRESH << 8) + ADRESL;  // Concat�ne les valeurs de ADRESH et ADRESL pour obtenir la valeur ADC
    sprintf("%d", potar);                         // Affiche la valeur du potentiom�tre (non utilis� ici)
    return (35 * potar) / 1023;                   // Convertit la valeur ADC en fonction de l'amplitude du potentiom�tre
}

// Cette fonction chiffre le caract�re re�u et l'envoie via la transmission s�rie
void chiffrer(void) {
    TX1REG = (char) ((int) RC1REG + offset);      // Ajoute l'offset au caract�re re�u et transmet le r�sultat
    Chiffrement = 1;                              // D�finit le mode de chiffrement � 1 (chiffrement activ�)
}

// Cette fonction d�chiffre le caract�re re�u et l'envoie via la transmission s�rie
void dechiffrer(void) {
    TX1REG = (char) ((int) RC1REG - offset);      // Soustrait l'offset au caract�re re�u et transmet le r�sultat
    Chiffrement = 0;                              // D�finit le mode de chiffrement � 0 (d�chiffrement activ�)
}

// Cette fonction convertit un nombre en deux caract�res ASCII repr�sentant ses unit�s et dizaines
void charToRien(char nombre) {
    char unite = (nombre % 10) + 48;              // Calcule le chiffre des unit�s et le convertit en caract�re ASCII
    char dizaines = (nombre / 10) + 48;           // Calcule le chiffre des dizaines et le convertit en caract�re ASCII
    String[0] = dizaines;                         // Stocke le caract�re repr�sentant les dizaines dans la premi�re position du tableau String
    String[1] = unite;                            // Stocke le caract�re repr�sentant les unit�s dans la deuxi�me position du tableau String
}

int cesar(int value, int offsetBis) {
    int renvoi;

    switch (Chiffrement) {                                // S�lectionne le mode de chiffrement ou de d�chiffrement
        case 1:                                           // Si le mode est chiffrement
            if (value < 58) {                             // Si la valeur est un chiffre (code ASCII 48-57)
                if (value + offsetBis >= 58) {            // Si le d�calage d�passe les chiffres
                    offsetBis = offsetBis - (58 - value); // Ajuste le d�calage pour �viter de sortir des chiffres
                    renvoi = cesar(97, offsetBis);        // Applique r�cursivement le chiffrement avec le d�calage ajust� et en utilisant 'a' (97) comme valeur de r�f�rence
                } else {
                    renvoi = value + offsetBis;           // Applique le d�calage sans d�passer la plage des chiffres
                }
            } else {                                      // Si la valeur est une lettre minuscule (code ASCII 97-122)
                if (value + offsetBis >= 123) {           // Si le d�calage d�passe les lettres minuscules
                    offsetBis = offsetBis - (123 - value);// Ajuste le d�calage pour �viter de sortir des lettres minuscules
                    renvoi = cesar(48, offsetBis);        // Applique r�cursivement le chiffrement avec le d�calage ajust� et en utilisant '0' (48) comme valeur de r�f�rence
                } else {
                    renvoi = value + offsetBis;           // Applique le d�calage sans d�passer la plage des lettres minuscules
                }
            }
            break;
        default:                                          // Si le mode est d�chiffrement
            if (value > 96) {                             // Si la valeur est une lettre minuscule (code ASCII 97-122)
                if (value - offsetBis <= 96) {            // Si le d�calage d�passe les lettres minuscules
                    offsetBis = offsetBis - (value - 96); // Ajuste le d�calage pour �viter de sortir des lettres minuscules
                    renvoi = cesar(57, offsetBis);        // Applique r�cursivement le d�chiffrement avec le d�calage ajust� et en utilisant '9' (57) comme valeur de r�f�rence
                } else {
                    renvoi = value - offsetBis;           // Applique le d�calage sans d�passer la plage des lettres minuscules
                }
            } else {                                      // Si la valeur est un chiffre (code ASCII 48-57)
                if (value - offsetBis <= 47) {            // Si le d�calage d�passe les chiffres
                    offsetBis = offsetBis - (value - 47); // Ajuste le d�calage pour �viter de sortir des chiffres
                    renvoi = cesar(122, offsetBis);       // Applique r�cursivement le d�chiffrement avec le d�calage ajust� et en utilisant 'z' (122) comme valeur de r�f�rence
                } else {
                    renvoi = value - offsetBis;           // Applique le d�calage sans d�passer la plage des chiffres
                }
            }
    }

    return renvoi;                                        // Retourne la valeur chiffr�e ou d�chiffr�e
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