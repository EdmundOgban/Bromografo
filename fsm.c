#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "cqueue.h"
#include "macros.h"
#include "fsm.h"
#include "fsm_definition.h"

#define INITIAL_STATE S_NAVIGATION

void fsm_init(struct fsm_s * fsm)
{
    queue_init(&fsm->signal_q, true);
    fsm->current_state = (struct fsm_state_s *)&INITIAL_STATE;
}

void fsm_emit_signal(struct fsm_s * fsm, fsm_signal_t trigger)
{
    queue_put(&fsm->signal_q, trigger);
}

#define foreach_signal(qp, lval) for (qel_t lval = queue_get(qp); lval != -1; lval = queue_get(qp))
void fsm_signal_handler(struct fsm_s * fsm)
{
    const struct fsm_transition_s * transtn;
    bool transition_done = false;

    foreach_signal(&fsm->signal_q, signal) {
        if (transition_done && IS_DISCARDABLE_SIGNAL(signal))
            continue;

        for (uint8_t i = 0; i < fsm->current_state->transitions_cnt; i++) {
            signal &= 0x3F;
            transtn = fsm->current_state->transitions[i];
            if (transtn->signal == signal) {
                SAFE_CALL_V(transtn->arc, signal);
                fsm->current_state = transtn->dst_state;
                transition_done = true;
                break;
            }
        }
    }
}

void fsm_run(struct fsm_s * fsm)
{
    SAFE_CALL(fsm->current_state->run);
}
