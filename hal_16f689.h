/* 
 * File:   hal.h
 * Author: enrico
 *
 * Created on 3 gennaio 2014, 14.38
 */

#ifndef HAL_H
#define	HAL_H

#define get_timer0() READTIMER0()
#define get_timer1() READTIMER1()

#define set_timer0(value) WRITETIMER0(value)
#define set_timer1(value) WRITETIMER1(value)

enum interrupts_list {
    GLOBAL,
    PERIPHERALS,
    INT_TIMER0,
    INT_TIMER1,
    IOC_RA0,
    IOC_RA1,
    IOC_RA2,
    IOC_RA3,
    IOC_RA4,
    IOC_RA5,
    IOC_RB4,
    IOC_RB5,
    IOC_RB6,
    IOC_RB7,
    IOC_PORTS,
    EXTERNAL_INT
};

void enable_interrupts(enum interrupts_list);
void disable_interrupts(enum interrupts_list);
void clear_interrupt(enum interrupts_list);


#endif	/* HAL_H */

