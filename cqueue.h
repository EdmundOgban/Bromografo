#include <string.h>
#include <stdint.h>
#include "fsm_signals.h"

#ifndef CQUEUE_H
#define CQUEUE_H

#define CQLEN 9
#define QUEUE_EMPTY -1

typedef fsm_signal_t qel_t;
typedef uint8_t      qpos_t;
typedef uint8_t      bit_t;

typedef struct {
     qel_t qels[CQLEN]; 
    qpos_t rp;
    qpos_t wp;
} cQ_s;

void queue_init(cQ_s *q, bit_t zerofill);
void queue_put(cQ_s *q, qel_t el);
qel_t queue_get(cQ_s *q);

#endif
