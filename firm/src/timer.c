#include "define.h"

#define TIMER_10us_TO_1ms ( 100 )

void timer_unit0_ch0_interrupt( void )
{
    static UBYTE timer_10us_to_1ms = 0;

    if ( timer_10us_to_1ms < UBYTE_MAX )
    {
        timer_10us_to_1ms++;
    }

    /* 10usec–ˆˆ— */
    {
        eeprom_sk_wait_10us_inc();
    }

    /* 1msec–ˆˆ— */
    if ( timer_10us_to_1ms >  TIMER_10us_TO_1ms )
    {
        timer_10us_to_1ms = 0;
        uart_trans_1ms_inc();
        eeprom_wait_1ms_inc();
    }
}
