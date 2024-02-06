#include "configbits.h" // Bits de configuration
#include <xc.h>         // Definition des registres specifiques au uC
#define DELAY 1
#define _XTAL_FREQ 8000000

void init() {
    //Assignation RX/TX
    RC6PPSbits.RC6PPS = 0x14; //10100 p.154
    RXPPSbits.RXPPS = 0x17; //10111 p.153

    //Configuration des pins RC6&RC7
    TRISCbits.TRISC6 = 0; //definie C6 en sortie
    TRISCbits.TRISC7 = 1; //definie C7 en entree
    ANSELCbits.ANSC7 = 0; //numerique

    //Configuration du Baudrate a 9600 bauds
    TX1STAbits.SYNC = 0; //p.363
    TX1STAbits.BRGH = 0; //low speed p.368
    BAUD1CONbits.BRG16 = 0;
    SP1BRGHbits.SP1BRGH = 0; //high bits
    SP1BRGLbits.SP1BRGL = 0x0C; //low bits

    //Activation du module EUSART
    RC1STAbits.SPEN = 1; //serial port enable
    TX1STAbits.TXEN = 1; //transmit enable
    RC1STAbits.CREN = 1; //enables receiver  

    //Configuration interrupt
    INTCONbits.GIE = 1; //enable all interrupts
    INTCONbits.PEIE = 1; //enable peripherals interrupts
    PIE1bits.RCIE = 1; //enable the USART receive interrupts
}

void __interrupt() isr(void) {
    if (PIE1bits.RCIE && PIR1bits.RCIF) {
        TX1REG = RC1REG;
    }
}

char tableau[] = {'H', 'e', 'l', 'l', 'o', ',', ' ', 'W', 'o', 'r', 'l', 'd', ' ', '!', '\n', '\r'};

void main(void) {
    /* Code d'initialisation */
    init();

    //Utilisation du module EUSART envoi d'un msg "HelloWorld" pour verifier que la carte communique bien en série
    __delay_ms(250);

    for (int i = 0; i < sizeof (tableau); i++) {
        TX1REG = tableau[i];
        __delay_ms(DELAY);

    }

    while (1) {

    }
}