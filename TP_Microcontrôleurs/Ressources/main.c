#include "configbits.h" // Bits de configuration
#include <xc.h>         // Definition des registres specifiques au uC


void init() {
    //Assignation RX/TX
    RC6PPSbits.RC6PPS = 0x14; //10100 p.154
    RXPPSbits.RXPPS = 0x17; //10111 p.153
    
    //Configuration des pins RC6&RC7
    TRISCbits.TRISC6 = 0; //définie C6 en sortie
    TRISCbits.TRISC7 = 1; //définie C7 en entrée
    ANSELCbits.ANSC7 = 0; //numerique
    
    //Configuration du Baudrate à 9600 bauds
    TX1STAbits.SYNC = 0; //p.363
    TX1STAbits.BRGH = 0; //low speed p.368
    BAUD1CONbits.BRG16 = 0;
    SP1BRGHbits.SP1BRGH = 0;//high bits
    SP1BRGLbits.SP1BRGL = 0x0C;//low bits
    
    //Activation du module EUSART
    RC1STAbits.SPEN = 1; //serial port enable
    TX1STAbits.TXEN = 1; //transmit enable
    RC1STAbits.CREN = 1; //enables receiver  
}

void main(void) {
    /* Code d'initialisation */
    init();
    
    char tableau[] = {'h', 'e', 'l', 'l', 'o', ',', ' ', 'w', 'o', 'r', 'l', 'd', '\n', '\r'};
        int i = 0;
         for (i = 0; i < sizeof(tableau) / sizeof(tableau[0]); i++) {
           TX1REG = tableau[i];
            for(int j = 0; j < 500; j++);
        
         }
    while(1){
        //Utilisation du module EUSART
        /*
        TX1REG = 'H'; //H

        TX1REG = 101; //e

    
        TX1REGbits.TX1REG = 108; //l

        TX1REGbits.TX1REG = 108; //l

        TX1REGbits.TX1REG = 111; //o

        TX1REGbits.TX1REG = 44; //,

        TX1REGbits.TX1REG = 87; //W

        TX1REGbits.TX1REG = 111; //o

        TX1REGbits.TX1REG = 114; //r

        TX1REGbits.TX1REG = 111; //l

        TX1REGbits.TX1REG = 100; //d

        TX1REGbits.TX1REG = 33; //!

        TX1REGbits.TX1REG = 92; //\ 

        TX1REGbits.TX1REG = 110; //n

        TX1REGbits.TX1REG = 92; //\ 

        TX1REGbits.TX1REG = 114; //r
        */
        
    }
}
