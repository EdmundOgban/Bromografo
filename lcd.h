/* 
 * File:   lcd.h
 * Author: enrico
 *
 * Created on 3 gennaio 2014, 13.48
 */

#include <stdint.h>
#include <stdbool.h>

#ifndef LCD_H
#define	LCD_H

#define LCD_LEN 16

#define DISP_CLEAR 0x01
#define DISP_RET_HOME 0x02

#define DISP_ENTRY_MODE_SET 0x04
#define DISP_CURSOR_INCREMENT 0x02
#define DISP_DATA_SHIFT 0x01

#define DISP_ON_OFF_CTL 0x08
#define DISP_ALL_ON 0x04
#define DISP_CURSOR_ON 0x02
#define DISP_BLINK_CURSOR_POS 0x01

#define DISP_SYSTEM_SET 0x20
#define DISP_IFACE_8_BIT 0x10
#define DISP_TWO_LINES 0x08
#define DISP_DOTS_5X10 0x04

#define DISP_SET_DDRAM_ADDR 0x80

// microseconds
#define tAS  1
#define tAH  1
#define tDSW 1
#define tH   1
#define tDDR 1
#define PWEH 1

#define lcd_refresh() lcd_cmd(DISP_ON_OFF_CTL | DISP_ALL_ON, true)

/*static const unsigned int8 lcd_mmap[LCD_LEN] = {
    0, 1, 2, 3, 4, 5, 6, 7, 64, 65, 66, 67, 68, 69, 70, 71
};*/

/*
static const unsigned int8 lcd_mmap[LCD_LEN] = {
    0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15,
    64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79,
    16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
    80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95
};
*/

/*
void lcd_puts(void);
void lcd_update(void);
 */
void lcd_putch(uint8_t, uint8_t, bool);
void lcd_puts(uint8_t *);
void lcd_init(void);
inline void lcd_clear(void);

#endif	/* LCD_H */

