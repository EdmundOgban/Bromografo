/* 
 * File:   interrupts.h
 * Author: Enrico
 *
 * Created on 25 agosto 2014, 0.09
 */

#ifndef INTERRUPTS_H
#define	INTERRUPTS_H

#define T0_CALIB 8
#define T1_CALIB 32768L

#define INP_GET_BTN_NEXT_PIN()  BIT_BOOL(PORTA, BTN_NEXT_PIN)
#define INP_GET_BTN_PREV_PIN()  BIT_BOOL(PORTA, BTN_PREV_PIN)
#define INP_GET_BTN_ENTER_PIN() BIT_BOOL(PORTA, BTN_ENTER_PIN)
#define INP_GET_DOOR_PIN()      BIT_BOOL(PORTA, DOOR_PIN)
#define INP_GET_VACM_PIN()      BIT_BOOL(PORTB, VACM_PIN)

#define CHECK_FOR_INPUT_FSMEMIT(inp, bitvar, signal_if_one, signal_if_zero)    \
    if (!(inp) && !(bitvar)) {                                                 \
        bitvar = 1;                                                            \
        fsm_emit_signal(&fsm, (signal_if_one));                                \
    }                                                                          \
    else if ((inp) && (bitvar)) {                                              \
        bitvar = 0;                                                            \
        fsm_emit_signal(&fsm, (signal_if_zero));                               \
    }

#define CHECK_FOR_INPUT_WHOLD(inp, bitvar, inh_if_not_hold)                    \
    if (!(inp) && !(bitvar)) {                                                 \
        bitvar = 1;                                                            \
        swtimer_enable(&swtimers, &tmr_button_hold);                           \
    }                                                                          \
    else if ((inp) && (bitvar)) {                                              \
        bitvar = 0;                                                            \
        if (!inputs.in_hold)                                                   \
            queue_put(&inh_q, (inh_if_not_hold));                              \
        inputs.in_hold = 0;                                                    \
        swtimer_disable(&tmr_button_hold);                                     \
    }

struct inputs_s {
    unsigned btn_next       : 1;
    unsigned btn_prev       : 1;
    unsigned btn_enter      : 1;
    unsigned door_state     : 1;
    unsigned vacuum_present : 1;
    unsigned in_hold        : 1;
    unsigned                : 2;
};

#endif	/* INTERRUPTS_H */

