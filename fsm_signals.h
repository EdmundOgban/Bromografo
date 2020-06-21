#ifndef _FSM_SIGNALS_H
#define _FSM_SIGNALS_H

#define IS_DISCARDABLE_SIGNAL(sig) ((sig & 0x40) ? 1 : 0)
#define DISCARDABLE(sig) (sig | 0x40)

typedef int8_t fsm_signal_t;

enum signals_e {
    BTN_ENTER_HOLD = 1,
    DOOR_CLOSE,
    DOOR_OPEN,
    RTC_CNT_END,
    VACUUM_OK,
    BTN_PREV_HOLD,
    VACUUM_NOK,
    ENTER_TEST_MENU,
};

#endif
