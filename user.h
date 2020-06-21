/******************************************************************************/
/* User Level #define Macros                                                  */
/******************************************************************************/

/******************************************************************************/
/* User Function Prototypes                                                   */
/******************************************************************************/
#ifndef _USER_H
#define _USER_H

#define TRISA_CONF   0x3F
#define TRISB_CONF   0x80
#define TRISC_CONF_I 0xF0
#define TRISC_CONF_O 0x00

#define T0_ENABLE           (1 << 7)
#define T0_8_BIT            (1 << 6)
#define T0_INC_ON_T0CKI     (1 << 5)
#define T0_HTOL_T0CKI       (1 << 4)
#define T0_BYPASS_PRESCALER (1 << 3)
#define T0_DIV_2             0
#define T0_DIV_4             1
#define T0_DIV_8             2
#define T0_DIV_16            3
#define T0_DIV_32            4
#define T0_DIV_64            5
#define T0_DIV_128           6
#define T0_DIV_256           7

#define T1_RD16             (1 << 7)
#define T1_CLK_INT1OSC      (1 << 6)
#define T1_DIV_1            (0 << 4)
#define T1_DIV_2            (1 << 4)
#define T1_DIV_4            (2 << 4)
#define T1_DIV_8            (3 << 4)
#define T1_OSC_ENABLE       (1 << 3)
#define T1_SYNC             (1 << 2)
#define T1_CLKSRC_EXT       (1 << 1)
#define T1_ENABLE           (1 << 0)

void peripheral_init(void);         /* I/O and Peripheral Initialization */

#endif