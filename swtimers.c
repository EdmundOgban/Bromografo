#include <xc.h>

#include <stdio.h>
#include <stdint.h>
#include "macros.h"
#include "swtimers.h"

inline void swtimer_enable(struct swtimers_s * swt, struct swt_handle_s * swh)
{
    swh->match = swt->tc + swh->period;
    swh->flags |= (SWT_ENABLED);
}

inline void swtimer_disable(struct swt_handle_s * swh)
{
    swh->flags &= ~(SWT_ENABLED);
}

inline void swtimer_configure(struct swt_handle_s * hdlr, uint8_t flags)
{
    hdlr->flags = flags;
}

inline void swtimer_set_period(struct swt_handle_s * hdlr, swt_cnt_t period)
{
    hdlr->period = period;
}

inline void swtimer_scan_for_match(struct swtimers_s * swt)
{
    struct swt_handle_s * hdlr;

    for (uint8_t idx = 0; idx < ALLOCATED_SWTIMERS; idx++) {
        hdlr = swt->timers[idx];
        if ((hdlr->flags & SWT_ENABLED) && hdlr->match == swt->tc)
            BIT_SET(hdlr->flags, SWT_TRIGGERED);
    }
}

void swtimer_call(struct swtimers_s * swt)
{
    struct swt_handle_s * hdlr;
    swt_cnt_t tc = swt->tc;

    for (uint8_t idx = 0; idx < ALLOCATED_SWTIMERS; idx++) {
        hdlr = swt->timers[idx];
        if (!(hdlr->flags & SWT_ENABLED) || !(hdlr->flags & SWT_TRIGGERED))
            continue;

        if (hdlr->flags & SWT_CYCLIC)
            hdlr->match = tc + hdlr->period;
        else
            swtimer_disable(hdlr);

        SAFE_CALL(hdlr->func);
        BIT_CLR(hdlr->flags, SWT_TRIGGERED);

    }
}
