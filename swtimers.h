/* 
 * File:   swtimers.h
 * Author: Enrico
 *
 * Created on 26 agosto 2014, 14.56
 */

#include <stdint.h>

#ifndef SWTIMERS_H
#define	SWTIMERS_H

#define ALLOCATED_SWTIMERS 5

#define SWT_NONE       0
#define SWT_ENABLED   (1 << 0)
#define SWT_TRIGGERED (1 << 1)
#define SWT_CYCLIC    (1 << 2)

typedef uint16_t swt_cnt_t;

struct swt_handle_s {
    swt_cnt_t match;
    swt_cnt_t period;
    uint8_t flags;
/*
    struct flags {
        unsigned enabled   : 1;
        unsigned triggered : 1;
        unsigned cyclic    : 1;
        unsigned           : 5;
    };
*/
    void (* func)(void);
};

struct swtimers_s {
    swt_cnt_t tc;
    struct swt_handle_s * timers[ALLOCATED_SWTIMERS];
};

enum inh_signals_e {
    INH_BTN_NEXT = 1,
    INH_BTN_NEXT_HOLD,
    INH_BTN_PREV,
    INH_BTN_PREV_HOLD,
    INH_BTN_ENTER,
    INH_BTN_ENTER_HOLD
};


inline void swtimer_enable(struct swtimers_s *, struct swt_handle_s *);
inline void swtimer_disable(struct swt_handle_s *);
inline void swtimer_configure(struct swt_handle_s *, uint8_t);
inline void swtimer_set_period(struct swt_handle_s *, swt_cnt_t);
inline void swtimer_scan_for_match(struct swtimers_s *);
void swtimer_call(struct swtimers_s *);

#endif	/* SWTIMERS_H */

