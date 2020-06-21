/* 
 * File:   main.h
 * Author: Enrico
 *
 * Created on 24 agosto 2014, 21.50
 */

#include <stdint.h>

#ifndef MAIN_H
#define	MAIN_H

#define MENU_ELEMENTS_CNT 3

struct clock_s {
    uint8_t mins;
    uint8_t secs;
};

struct globaldata_s {
    struct clock_s * cur_clock;
    uint8_t ** menu_elms;
    uint8_t  * buf;
    uint8_t    menu_pos;
    uint8_t    cur_pos;
    uint8_t    clock_edit;
};

#endif	/* MAIN_H */
