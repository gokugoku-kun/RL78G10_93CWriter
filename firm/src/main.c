/***********************************************************************/
/*                                                                     */
/*  FILE        :Main.c                                                */
/*  DATE        :                                                      */
/*  DESCRIPTION :Main Program                                          */
/*  CPU TYPE    :                                                      */
/*                                                                     */
/*  NOTE:THIS IS A TYPICAL EXAMPLE.                                    */
/*                                                                     */
/***********************************************************************/
#include "define.h"

void main(void);

void main(void)
{
    __DI();
    micon_resource_initialize();
    uart_recieve_initialize();
    uart_trans_initialize( TRUE );
    __EI();

    while ( TRUE )
    {
        uart_recieve_get_data();
        state_machine_main();
        led_main();
        uart_trans_set_data();
    }
}
