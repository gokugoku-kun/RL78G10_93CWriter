#include "define.h"

/**********************/
/*                    */
/* 割込ベクタテーブル */
/*                    */
/**********************/
#pragma interrupt VECTOR_INTWDTI  ( vect = INTWDTI  )
#pragma interrupt VECTOR_INTP0    ( vect = INTP0    )
#pragma interrupt VECTOR_INTP1    ( vect = INTP1    )
//#pragma interrupt VECTOR_INTCSI00 ( vect = INTCSI00 )
//#pragma interrupt VECTOR_INTIIC00 ( vect = INTIIC00 )
#pragma interrupt VECTOR_INTST0   ( vect = INTST0   )
//#pragma interrupt VECTOR_INTCSI01 ( vect = INTCSI01 )
#pragma interrupt VECTOR_INTSR0   ( vect = INTSR0   )
#pragma interrupt VECTOR_INTSRE0  ( vect = INTSRE0  )
#pragma interrupt VECTOR_INTTM01H ( vect = INTTM01H )
#pragma interrupt VECTOR_INTTM00  ( vect = INTTM00  )
#pragma interrupt VECTOR_INTTM01  ( vect = INTTM01  )
#pragma interrupt VECTOR_INTAD    ( vect = INTAD    )
#pragma interrupt VECTOR_INTKR    ( vect = INTKR    )
#pragma interrupt VECTOR_INTP2    ( vect = INTP2    )
#pragma interrupt VECTOR_INTP3    ( vect = INTP3    )
#pragma interrupt VECTOR_INTTM03H ( vect = INTTM03H )
#pragma interrupt VECTOR_INTIICA0 ( vect = INTIICA0 )
#pragma interrupt VECTOR_INTTM02  ( vect = INTTM02  )
#pragma interrupt VECTOR_INTTM03  ( vect = INTTM03  )
#pragma interrupt VECTOR_INTIT    ( vect = INTIT    )
#pragma interrupt VECTOR_INTCMP0  ( vect = INTCMP0  )

void VECTOR_INTWDTI  ( void ) {}
void VECTOR_INTP0    ( void ) {}
void VECTOR_INTP1    ( void ) {}
void VECTOR_INTCSI00 ( void ) {}
void VECTOR_INTIIC00 ( void ) {}
void VECTOR_INTST0   ( void )
{
    uart_trans_interrupt();
}
void VECTOR_INTCSI01 ( void ) {}
void VECTOR_INTSR0   ( void )
{
    uart_recieve_interrupt();
}
void VECTOR_INTSRE0  ( void )
{
    uart_recieve_error_interrupt();
}
void VECTOR_INTTM01H ( void ) {}
void VECTOR_INTTM00  ( void ) 
{
    timer_unit0_ch0_interrupt();
}
void VECTOR_INTTM01  ( void ) {}
void VECTOR_INTAD    ( void ) {}
void VECTOR_INTKR    ( void ) {}
void VECTOR_INTP2    ( void ) {}
void VECTOR_INTP3    ( void ) {}
void VECTOR_INTTM03H ( void ) {}
void VECTOR_INTIICA0 ( void ) {}
void VECTOR_INTTM02  ( void ) {}
void VECTOR_INTTM03  ( void ) {}
void VECTOR_INTIT    ( void ) {}
void VECTOR_INTCMP0  ( void ) {}
