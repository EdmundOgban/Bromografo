#include "system.h"

#ifndef _MACROS_H
#define	_MACROS_H

// Call a function pointer only if it's not NULL
#define SAFE_CALL(m)       if ((m) != NULL) (*(m))()
#define SAFE_CALL_V(m, v)  if ((m) != NULL) (*(m))(v)

#define BIT_GET(reg, msk)  ((reg) & (msk))
#define BIT_SET(reg, msk)  reg |= (msk)
#define BIT_CLR(reg, msk)  reg &= ~(msk)
#define BIT_BOOL(dst, msk) (BIT_GET(dst, msk) ? 1 : 0)
#define BIT_CPY(dst, bit)  (bit & 1 ? dst |= bit : dst &= ~(bit));

#define delay_msecs(val) for (uint16_t i = 0; i < val; i++) __delay_us(1000);

#endif	/* _MACROS_H */

