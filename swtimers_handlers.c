#include <xc.h>
#include <stdio.h>

#include "macros.h"
#include "pins.h"
#include "fsm.h"
#include "cqueue.h"
#include "lcd.h"
#include "hal.h"
#include "interrupts.h"
#include "swtimers.h"
#include "main.h"
#include "strings.h"

#define BUTTONS_COUNT 5
#define DEBOUNCED(B) ((debounce[B] & 0xE0) ? 1 : 0)

enum debounce_btns_e {
    DEBOUNCE_BTN_NEXT,
    DEBOUNCE_BTN_PREV,
    DEBOUNCE_BTN_ENTER,
    DEBOUNCE_DOOR,
    DEBOUNCE_VACUUM
};

extern struct swtimers_s swtimers;
extern struct fsm_s fsm;
extern cQ_s inh_q;
extern struct globaldata_s gdata;


volatile struct inputs_s inputs = { 0, 0, 0, 0, 0, 0 };
volatile uint8_t debounce[BUTTONS_COUNT] = { 0, 0, 0, 0, 0 };

void cyclic_display_refresh(void);
void do_buttons_hold(void);
void do_poll_inputs(void);
void do_preheat(void);
void do_beep_alarm(void);

struct swt_handle_s tmr_poll_inputs  = { 1,    10, (SWT_ENABLED | SWT_CYCLIC), do_poll_inputs };
struct swt_handle_s tmr_disp_refresh = { 1,   500, (SWT_CYCLIC),   cyclic_display_refresh };
struct swt_handle_s tmr_button_hold  = { 1,  2000, (SWT_NONE),     do_buttons_hold };
struct swt_handle_s tmr_preheat      = { 1,  3000, (SWT_NONE),     do_preheat };
struct swt_handle_s tmr_beep         = { 1,    50, (SWT_NONE),     do_beep_alarm };
struct swtimers_s swtimers = { 0,
                              { &tmr_poll_inputs, &tmr_disp_refresh,
                                &tmr_button_hold, &tmr_preheat,
                                &tmr_beep }
                             };




void cyclic_display_refresh(void)
{
    sprintf(gdata.buf, counter_str,
            gdata.cur_clock->mins, gdata.cur_clock->secs);
    lcd_clear();
    lcd_puts(gdata.buf);
}

void do_buttons_hold(void)
{
    if (inputs.btn_next) {
        queue_put(&inh_q, INH_BTN_NEXT_HOLD);
        inputs.btn_next = 1;
    }
    if (inputs.btn_prev) {
        queue_put(&inh_q, INH_BTN_PREV_HOLD);
        inputs.btn_prev = 1;
    }
    if (inputs.btn_enter) {
        queue_put(&inh_q, INH_BTN_ENTER_HOLD);
        inputs.btn_enter = 1;
    }
    inputs.in_hold = 1;
}

void do_poll_inputs(void)
{
    BIT_CPY(debounce[DEBOUNCE_BTN_NEXT], INP_GET_BTN_NEXT_PIN());
    debounce[DEBOUNCE_BTN_NEXT] <<= 1;

    BIT_CPY(debounce[DEBOUNCE_BTN_PREV], INP_GET_BTN_PREV_PIN());
    debounce[DEBOUNCE_BTN_PREV] <<= 1;

    BIT_CPY(debounce[DEBOUNCE_BTN_ENTER], INP_GET_BTN_ENTER_PIN());
    debounce[DEBOUNCE_BTN_ENTER] <<= 1;

    BIT_CPY(debounce[DEBOUNCE_DOOR], INP_GET_DOOR_PIN());
    debounce[DEBOUNCE_DOOR] <<= 1;

    BIT_CPY(debounce[DEBOUNCE_VACUUM], INP_GET_VACM_PIN());
    debounce[DEBOUNCE_VACUUM] <<= 1;


    CHECK_FOR_INPUT_WHOLD(DEBOUNCED(DEBOUNCE_BTN_NEXT) , inputs.btn_next,
                          INH_BTN_NEXT)
    CHECK_FOR_INPUT_WHOLD(DEBOUNCED(DEBOUNCE_BTN_PREV) , inputs.btn_prev,
                          INH_BTN_PREV)
    CHECK_FOR_INPUT_WHOLD(DEBOUNCED(DEBOUNCE_BTN_ENTER), inputs.btn_enter,
                          INH_BTN_ENTER)

    CHECK_FOR_INPUT_FSMEMIT(DEBOUNCED(DEBOUNCE_DOOR)  , inputs.door_state,
                            DOOR_OPEN                 , DOOR_CLOSE)
    CHECK_FOR_INPUT_FSMEMIT(DEBOUNCED(DEBOUNCE_VACUUM), inputs.vacuum_present,
                            VACUUM_OK                 , VACUUM_NOK)
}

void do_preheat(void)
{
    fsm_emit_signal(&fsm, RTC_CNT_END);
}

uint8_t beep_alarm_pos = 0;
void do_beep_alarm(void)
{
    uint16_t beeps_length[] = { 100, 250, 100, 1000 };
    uint8_t beeps_num = sizeof(beeps_length) / sizeof(uint16_t);

    swtimer_set_period(&tmr_beep, beeps_length[beep_alarm_pos++ % beeps_num]);
    swtimer_enable(&swtimers, &tmr_beep);

    if (BIT_GET(PORTB, BUZZ))
        BIT_CLR(PORTB, BUZZ);
    else
        BIT_SET(PORTB, BUZZ);
}


