#include <stdbool.h>
#include "cqueue.h"

#define SHORTCIRCUIT_INC_RP(q) if ((q)->rp == CQLEN-1) \
                                   (q)->rp = 0; \
                               else \
                                   (q)->rp++
#define SHORTCIRCUIT_INC_WP(q) if ((q)->wp == CQLEN-1) \
                                   (q)->wp = 0; \
                               else \
                                   (q)->wp++

void queue_init(cQ_s * q, bool zerofill)
{
    q->rp = 0;
    q->wp = 0;

    if (zerofill)
        memset(q->qels, 0, CQLEN*sizeof(qel_t));
}

void queue_put(cQ_s * q, qel_t el)
{
    q->qels[q->wp] = el;

    SHORTCIRCUIT_INC_WP(q);

    // If queue is full, move rp one step ahead
    if (q->wp == q->rp)
        SHORTCIRCUIT_INC_RP(q);
}

void queue_put_fromstr(cQ_s * q, uint8_t * s)
{
    while (*s)
        queue_put(q, *s++);
}

qel_t queue_get(cQ_s * q)
{
    qel_t tmp;

    if (q->rp == q->wp)
        return -1;

    tmp = q->qels[q->rp];
    SHORTCIRCUIT_INC_RP(q);

    return tmp;
}
