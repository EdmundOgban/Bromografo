/*
 * File:   hal.c
 * Author: enrico
 *
 * Created on 3 gennaio 2014, 14.38
 */


#include <xc.h>
#include <stdint.h>

#include "hal_16f689.h"

void enable_interrupts(enum interrupts_list intr)
{
    switch (intr) {
        case GLOBAL:
            ei();
            break;
        case PERIPHERALS:
            PEIE = 1;
            break;
        case INT_TIMER0:
            T0IE = 1;
            break;
        case INT_TIMER1:
            T1IE = 1;
            break;
        case IOC_RA0:
            IOCA0 = 1;
            break;
        case IOC_RA1:
            IOCA1 = 1;
            break;
        case IOC_RA2:
            IOCA2 = 1;
            break;
        case IOC_RA3:
            IOCA3 = 1;
            break;
        case IOC_RA4:
            IOCA4 = 1;
            break;
        case IOC_RA5:
            IOCA5 = 1;
            break;
        case IOC_RB4:
            IOCB4 = 1;
            break;
        case IOC_RB5:
            IOCB5 = 1;
            break;
        case IOC_RB6:
            IOCB6 = 1;
            break;
        case IOC_RB7:
            IOCB7 = 1;
            break;
        case IOC_PORTS:
            RABIE = 1;
            break;
        case EXTERNAL_INT:
            INTE = 1;
            break;
    }
}

void disable_interrupts(enum interrupts_list intr)
{
    switch (intr) {
        case GLOBAL:
            di();
            break;
        case PERIPHERALS:
            PEIE = 0;
            break;
        case INT_TIMER0:
            T0IE = 0;
            break;
        case INT_TIMER1:
            T1IE = 0;
            break;
        case IOC_RA0:
            IOCA0 = 0;
            break;
        case IOC_RA1:
            IOCA1 = 0;
            break;
        case IOC_RA2:
            IOCA2 = 0;
            break;
        case IOC_RA3:
            IOCA3 = 0;
            break;
        case IOC_RA4:
            IOCA4 = 0;
            break;
        case IOC_RA5:
            IOCA5 = 0;
            break;
        case IOC_RB4:
            IOCB4 = 0;
            break;
        case IOC_RB5:
            IOCB5 = 0;
            break;
        case IOC_RB6:
            IOCB6 = 0;
            break;
        case IOC_RB7:
            IOCB7 = 0;
            break;
        case IOC_PORTS:
            RABIE = 0;
            break;
        case EXTERNAL_INT:
            INTE = 0;
            break;
    }
}

void clear_interrupt(enum interrupts_list intr)
{
    switch (intr) {
        case INT_TIMER0:
            T0IF = 0;
            break;
        case INT_TIMER1:
            T1IF = 0;
            break;
        case IOC_PORTS:
            RABIF = 0;
            break;
        case EXTERNAL_INT:
            INTF = 0;
            break;
    }

}

