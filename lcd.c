/*
 * File:   lcd.c
 * Author: enrico
 *
 * Created on 3 gennaio 2014, 13.46
 */

#include <xc.h>
#include <stdint.h>
#include <string.h>

#include "user.h"
#include "system.h"
#include "macros.h"
#include "pins.h"
#include "lcd.h"

inline static void check_busy_flag(void)
{
    BIT_CLR(PORTC, BSYF);
    BIT_CLR(PORTC, RS);
    BIT_SET(PORTC, RW);
    __delay_us(tAS);
    BIT_SET(PORTC, EI);
    __delay_us(tDDR);

    TRISC = TRISC_CONF_I;
    while (BIT_GET(PORTC, BSYF));
    TRISC = TRISC_CONF_O;

    BIT_CLR(PORTC, EI);
    __delay_us(tAH);
    BIT_CLR(PORTC, RW);
}

inline static void enable_input(void)
{
    BIT_SET(PORTC, EI);
    __delay_us(PWEH);
    BIT_CLR(PORTC, EI);
    __delay_us(tAH);
}

static void lcd_cmd(uint8_t cmd, bool check_bf)
{
    BIT_CLR(PORTC, 0xF0);
    BIT_SET(PORTC, cmd & 0xF0);
    __delay_us(1);
    enable_input();

    BIT_CLR(PORTC, 0xF0);
    BIT_SET(PORTC, (cmd & 0x0F) << 4);
    __delay_us(1);
    enable_input();

    if (check_bf)
        check_busy_flag();

    __delay_us(1);
}

inline static void lcd_write_datareg(uint8_t chr)
{
    BIT_SET(PORTC, RS);
    lcd_cmd(chr, true);
    BIT_CLR(PORTC, RS);
}

inline static void lcd_write_instreg(uint8_t chr, bool check_bf)
{
    BIT_CLR(PORTC, RS);
    lcd_cmd(chr, check_bf);
}

inline void lcd_clear(void)
{
    lcd_write_instreg(DISP_CLEAR, true);
}

void lcd_putch(uint8_t chr, uint8_t pos, bool disp_refresh)
{
    lcd_write_instreg(DISP_SET_DDRAM_ADDR | (pos > 7 ? pos + 56 : pos), true);
    __delay_us(100);
    lcd_write_datareg(chr);
    __delay_us(48);

    if (disp_refresh)
        lcd_refresh();
}

void lcd_puts(uint8_t * str)
{
    uint8_t i = 0;

    for (; *str; str++, i = (i + 1) % LCD_LEN)
        lcd_putch(*str, i, false);

    lcd_refresh();
}

void lcd_init(void)
{
    delay_msecs(100);

    lcd_write_instreg(0x30, false);
    delay_msecs(6);

    lcd_write_instreg(0x30, false);
    __delay_us(150);

    lcd_write_instreg(0x30, false);
    __delay_us(55);

    lcd_write_instreg(DISP_SYSTEM_SET, true);
    lcd_write_instreg(DISP_ON_OFF_CTL, true);
    lcd_clear();
    lcd_write_instreg(DISP_ENTRY_MODE_SET, true);

    delay_msecs(100);
}
