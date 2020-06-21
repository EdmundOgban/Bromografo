#include "cqueue.h"
#include "fsm_signals.h"

#ifndef _FSM_H
#define _FSM_H

struct fsm_transition_s {
    fsm_signal_t signal;
    const struct fsm_state_s * dst_state;
    void (* arc)(fsm_signal_t);
};

struct fsm_state_s {
    uint8_t id;
    void (* run)(void);
    uint8_t transitions_cnt;
    const struct fsm_transition_s ** transitions;
};

struct fsm_s {
    const struct fsm_state_s * current_state;
    cQ_s signal_q;
};

void fsm_init(struct fsm_s *);
void fsm_emit_signal(struct fsm_s *, fsm_signal_t);

void fsm_signal_handler(struct fsm_s *);
void fsm_run(struct fsm_s *);

#endif
