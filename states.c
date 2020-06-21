/* 
 * File:   states.c
 * Author: Enrico
 *
 * Created on 28 agosto 2014, 21.41
 */

#include <xc.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "macros.h"
#include "pins.h"
#include "lcd.h"
#include "fsm.h"
#include "hal.h"
#include "interrupts.h"
#include "main.h"
#include "cqueue.h"
#include "strings.h"
#include "swtimers.h"

extern struct swtimers_s swtimers;
extern struct swt_handle_s tmr_disp_refresh;
extern struct swt_handle_s tmr_preheat;
extern struct swt_handle_s tmr_beep;
extern struct fsm_s fsm;
extern struct clock_s clocks[];
extern volatile struct globaldata_s gdata;
extern uint8_t * menus[];
extern cQ_s inh_q;
//extern uint8_t buf[];

volatile struct s_tdata {
    uint8_t  portnum;
    uint8_t *portaddr;
    uint8_t *trisaddr;
    uint8_t  editing;
} tdata = { 0, 0x0F80, 0x0F92, 0 };

char *brepr(char *buf, uint8_t num)
{
    int8_t i, j;

    for (i = 0, j = 7; i < 8, j >= 0; i++, j--) {
        buf[i] = (num & (1 << j)) ? '1' : '0';
    }
    buf[8] = 0;

    return buf;
}

void fsm_enter(void)
{
    char lbuf[9];

    if (!INP_GET_BTN_NEXT_PIN() && !INP_GET_BTN_ENTER_PIN()) {
        sprintf(gdata.buf, "PORT%c: %s",
                'A'+tdata.portnum, brepr(lbuf, *tdata.portaddr));
        fsm_emit_signal(&fsm, ENTER_TEST_MENU);
    }
    else {
        sprintf(gdata.buf, navigation_str,
                gdata.menu_pos+1, menus[gdata.menu_pos]);
    }

    lcd_clear();
    lcd_puts(gdata.buf);
}

void s_vacuum_lost__run(void)
{
    BIT_CLR(PORTB, LAMP);
    BIT_CLR(PORTB, PUMP);
}

void s_navigation__run(void)
{
    qel_t ev;

    BIT_CLR(PORTB, LAMP);
    BIT_CLR(PORTB, PUMP);
    BIT_CLR(PORTB, BUZZ);

    while ((ev = queue_get(&inh_q)) != QUEUE_EMPTY) {
        switch (ev) {
            case INH_BTN_NEXT:
                gdata.menu_pos++;
                if (gdata.menu_pos == MENU_ELEMENTS_CNT)
                    gdata.menu_pos = 0;
                break;
            case INH_BTN_ENTER:
                break;
            case INH_BTN_PREV:
                gdata.menu_pos--;
                if (gdata.menu_pos > MENU_ELEMENTS_CNT)
                    gdata.menu_pos = MENU_ELEMENTS_CNT - 1;
                break;
            case INH_BTN_ENTER_HOLD:
                fsm_emit_signal(&fsm, BTN_ENTER_HOLD);
                goto break_outer;
        }
        sprintf(gdata.buf, navigation_str,
                gdata.menu_pos+1, menus[gdata.menu_pos]);
        lcd_clear();
        lcd_puts(gdata.buf);
        continue;
break_outer:
        break;
    }

}
void s_set_counter__run(void)
{
    qel_t ev;

    BIT_CLR(PORTB, LAMP);
    BIT_CLR(PORTB, PUMP);
    BIT_CLR(PORTB, BUZZ);

    while ((ev = queue_get(&inh_q)) != QUEUE_EMPTY) {
        switch (ev) {
            case INH_BTN_NEXT:
                if (gdata.clock_edit) {
                    gdata.buf[gdata.cur_pos]++;

                    if (gdata.cur_pos == 8) {
                        if (gdata.buf[gdata.cur_pos] > '5')
                            gdata.buf[gdata.cur_pos] = '0';
                    }
                    else {
                        if (gdata.buf[gdata.cur_pos] > '9')
                            gdata.buf[gdata.cur_pos] = '0';
                    }

                    lcd_putch(gdata.buf[gdata.cur_pos],
                              gdata.cur_pos, true);
                }
                else {
                    lcd_putch(gdata.buf[gdata.cur_pos],
                              gdata.cur_pos, true);
                    gdata.cur_pos++;
                    if (gdata.cur_pos > 9)
                        gdata.cur_pos = 5;
                    if (gdata.cur_pos == 7)
                        gdata.cur_pos++;
                    lcd_putch('_', gdata.cur_pos, true);
                }

                break;
            case INH_BTN_ENTER: {
                uint8_t c;

                if (gdata.clock_edit) {
                    gdata.clock_edit = 0;
                    c = '_';
                }
                else {
                    gdata.clock_edit = 1;
                    c = '*';
                }

                lcd_putch(c, gdata.cur_pos, true);

                break;
            }
            case INH_BTN_PREV:
                if (gdata.clock_edit) {
                    gdata.buf[gdata.cur_pos]--;

                    if (gdata.cur_pos == 8) {
                        if (gdata.buf[gdata.cur_pos] < '0')
                            gdata.buf[gdata.cur_pos] = '5';
                    }
                    else {
                        if (gdata.buf[gdata.cur_pos] < '0')
                            gdata.buf[gdata.cur_pos] = '9';
                    }

                    lcd_putch(gdata.buf[gdata.cur_pos],
                              gdata.cur_pos, true);
                }
                else {
                    lcd_putch(gdata.buf[gdata.cur_pos],
                              gdata.cur_pos, true);
                    gdata.cur_pos--;
                    if (gdata.cur_pos < 5)
                        gdata.cur_pos = 9;
                    if (gdata.cur_pos == 7)
                        gdata.cur_pos--;
                    lcd_putch('_', gdata.cur_pos, true);
                }
 

                break;
            case INH_BTN_ENTER_HOLD:
                gdata.clock_edit = 0;
                fsm_emit_signal(&fsm, BTN_ENTER_HOLD);

                break;
        }
    }

}
void s_holdpcb_door__run(void)
{
    BIT_CLR(PORTB, LAMP);
    BIT_SET(PORTB, PUMP);
    BIT_CLR(PORTB, BUZZ);
}
void s_holdpcb__run(void)
{
    BIT_SET(PORTB, LAMP);
    BIT_SET(PORTB, PUMP);
    BIT_CLR(PORTB, BUZZ);
}
void s_close_for_start__run(void)
{
    BIT_SET(PORTB, LAMP);
    BIT_SET(PORTB, PUMP);
    BIT_CLR(PORTB, BUZZ);
}
void s_countdown_done__run(void)
{
    BIT_CLR(PORTB, LAMP);
    BIT_CLR(PORTB, PUMP);
}
void s_lamp_preheat__run(void)
{
    BIT_SET(PORTB, LAMP);
    BIT_CLR(PORTB, PUMP);
    BIT_CLR(PORTB, BUZZ);
}
void s_door_opened__run(void)
{
    BIT_CLR(PORTB, LAMP);
    BIT_SET(PORTB, PUMP);
}
void s_countdown__run(void)
{
    BIT_SET(PORTB, LAMP);
    BIT_SET(PORTB, PUMP);
    BIT_CLR(PORTB, BUZZ);
}

void do_testmenu_enter(void)
{
    tdata.editing = (tdata.editing + 1) % 3;

    if (tdata.editing == 1) {
        gdata.cur_pos = 7;
        lcd_putch('_', gdata.cur_pos, true);
    }
}

void do_testmenu_up(void)
{
    uint8_t bytepos;
    char lbuf[9];

    switch (tdata.editing) {
        case 0: // Navigation through the ports
            if (tdata.portnum == 2) {
                tdata.portnum = 0;
                tdata.portaddr = 0x0F80;
                tdata.trisaddr = 0x0F92;
            }
            else {
                tdata.portnum++;
                tdata.portaddr++;
                tdata.trisaddr++;
            }
            sprintf(gdata.buf, "PORT%c: %s",
                    'A'+tdata.portnum, brepr(lbuf, *tdata.portaddr));
            lcd_clear();
            lcd_puts(gdata.buf);

            break;
        case 1: // Navigation through the bits
            lcd_putch(gdata.buf[gdata.cur_pos], gdata.cur_pos, true);
            if (++gdata.cur_pos > 14)
                gdata.cur_pos = 7;
            lcd_putch('_', gdata.cur_pos, true);

            break;
        case 2: // Bit changing
            bytepos = 7-(gdata.cur_pos-7);

            if (!(*tdata.trisaddr & (1 << bytepos))) { // TRISx 0 is output
                if (gdata.buf[gdata.cur_pos] == '0') {
                    BIT_SET(*tdata.portaddr, 1 << bytepos);
                    gdata.buf[gdata.cur_pos] = '1';
                }
                else {
                    BIT_CLR(*tdata.portaddr, 1 << bytepos);
                    gdata.buf[gdata.cur_pos] = '0';
                }
                lcd_putch(gdata.buf[gdata.cur_pos], gdata.cur_pos, true);
            }
            break;
    }

}

void do_testmenu_down(void)
{
    char lbuf[9];

    switch (tdata.editing) {
        case 0: // Navigation through the ports
            if (tdata.portnum == 0) {
                tdata.portnum = 2;
                tdata.portaddr = 0x0F82;
                tdata.trisaddr = 0x0F94;
            }
            else {
                tdata.portaddr--;
                tdata.portnum--;
                tdata.trisaddr--;
            }

            sprintf(gdata.buf, "PORT%c: %s",
                    'A'+tdata.portnum, brepr(lbuf, *tdata.portaddr));
            lcd_clear();
            lcd_puts(gdata.buf);

            break;
        case 1: // Navigation through the bits
            lcd_putch(gdata.buf[gdata.cur_pos], gdata.cur_pos, false);
            if (--gdata.cur_pos < 7)
                gdata.cur_pos = 14;
            lcd_putch('_', gdata.cur_pos, true);

            break;
        case 2: // Bit changing
            tdata.editing = 1;
            break;
    }

}

void s_testmenu__run(void)
{
    qel_t ev;

    while ((ev = queue_get(&inh_q)) != QUEUE_EMPTY) {
        switch (ev) {
            case INH_BTN_PREV:
                do_testmenu_down();
                break;
            case INH_BTN_ENTER:
                do_testmenu_enter();
                break;
            case INH_BTN_NEXT:
                do_testmenu_up();
                break;
            case INH_BTN_PREV_HOLD:
                fsm_emit_signal(&fsm, BTN_PREV_HOLD);
                break;
        }
    }
}


void btn_prev_hold__arc_common(fsm_signal_t trigger)
{
    disable_interrupts(INT_TIMER1);
    swtimer_disable(&tmr_disp_refresh);

    swtimer_disable(&tmr_beep);
    fsm_enter();
}

void btn_enter_hold__arc_common(fsm_signal_t trigger)
{
    btn_prev_hold__arc_common(trigger);
}

void s_navigation__arc_to__s_set_counter(fsm_signal_t trigger)
{
    gdata.cur_clock = &clocks[gdata.menu_pos];
    gdata.cur_pos = 5;
    sprintf(gdata.buf, counter_str,
            gdata.cur_clock->mins, gdata.cur_clock->secs);
    lcd_clear();
    lcd_puts(gdata.buf);
}

void s_set_counter__arc_to__s_lamp_preheat(fsm_signal_t trigger)
{
    gdata.cur_clock->mins = 0;
    gdata.cur_clock->secs = 0;

    for (uint8_t i = 5; i <= 9; i++) {
       switch (i) {
           case 5:
           case 6:
               gdata.cur_clock->mins += ((gdata.buf[i] - '0') * (i == 5 ? 10 : 1));
               break;
           case 8:
           case 9:
               gdata.cur_clock->secs += ((gdata.buf[i] - '0') * (i == 8 ? 10 : 1));
               break;
       }
    }

    strcpy(gdata.buf, lamp_heat_str);
    swtimer_enable(&swtimers, &tmr_preheat);
    lcd_clear();
    lcd_puts(gdata.buf);
}

void s_holdpcb__arc_to__s_holdpcb_door(fsm_signal_t trigger)
{
    
}

void s_holdpcb_door__arc_to__s_door_opened(fsm_signal_t trigger)
{
    strcpy(gdata.buf, door_open_str);
    lcd_clear();
    lcd_puts(gdata.buf);
}

void s_lamp_preheat__arc_to__s_holdpcb(fsm_signal_t trigger)
{
    strcpy(gdata.buf, pcb_hold_str);
    lcd_clear();
    lcd_puts(gdata.buf);
}

void s_holdpcb_door__arc_to__s_holdpcb(fsm_signal_t trigger)
{
    //s_lamp_preheat__arc_to__s_holdpcb(trigger);
}

void s_countdown_done__arc_to__s_navigation(fsm_signal_t trigger)
{
    if (trigger == DOOR_OPEN)
        btn_prev_hold__arc_common(trigger);
}

void s_lamp_preheat__arc_to__s_set_counter(fsm_signal_t trigger)
{
    s_navigation__arc_to__s_set_counter(trigger);
}

void s_holdpcb__arc_to__s_countdown(fsm_signal_t trigger)
{
    swtimer_enable(&swtimers, &tmr_disp_refresh);
    enable_interrupts(INT_TIMER1);

    sprintf(gdata.buf, counter_str, gdata.cur_clock->mins, gdata.cur_clock->secs);
    lcd_clear();
    lcd_puts(gdata.buf);
}

void s_door_opened__arc_to__s_countdown(fsm_signal_t trigger)
{
    s_holdpcb__arc_to__s_countdown(trigger);
}

extern uint8_t beep_alarm_pos;
void vacuum_nok__arc_common(fsm_signal_t trigger)
{
    swtimer_disable(&tmr_disp_refresh);
    disable_interrupts(INT_TIMER1);

    beep_alarm_pos = 0;
    swtimer_set_period(&tmr_beep, 50);
    swtimer_enable(&swtimers, &tmr_beep);

    strcpy(gdata.buf, lost_vacuum_str);
    lcd_clear();
    lcd_puts(gdata.buf);
}

void s_countdown__arc_to__s_door_opened(fsm_signal_t trigger)
{
    swtimer_disable(&tmr_disp_refresh);
    disable_interrupts(INT_TIMER1);

    strcpy(gdata.buf, door_open_str);
    lcd_clear();
    lcd_puts(gdata.buf);
}

void s_countdown__arc_to__s_countdown_done(fsm_signal_t trigger)
{
    swtimer_disable(&tmr_disp_refresh);
    disable_interrupts(INT_TIMER1);

    beep_alarm_pos = 0;
    swtimer_set_period(&tmr_beep, 50);
    swtimer_enable(&swtimers, &tmr_beep);
}

