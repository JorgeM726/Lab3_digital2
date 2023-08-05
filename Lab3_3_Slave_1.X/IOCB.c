/*
 * File:   IOCB.c
 * Author: jmuni
 *
 * Created on July 18, 2023, 3:57 PM
 */


#include <xc.h>
#include <stdint.h>
#include "IOCB.h"
//Ingresar como 1 los pines para los cuales se desea activar la interrupción on change. El resto de pines serán configurados como salida
ioc_init(char pin){ 
    TRISB = pin;
    WPUB = pin;
    IOCB = pin;
    PORTB=0;
    OPTION_REGbits.nRBPU=0; 
    INTCONbits.RBIE=1; 
    INTCONbits.RBIF=0; 
}

