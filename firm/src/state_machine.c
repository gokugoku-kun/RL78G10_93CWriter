#include "define.h"

static UBYTE state_get_new( UBYTE now );
/* 定常処理 */
static void state_do_wait( void );
static void state_do_write( void );
static void state_do_read( void );
/* 退場処理 */
static void state_exit_write( void );
/* 入場処理 */
static void state_entry_write( void );
static void state_entry_read( void );
static void state_entry_write_finish( void );
static void state_entry_read_finish( void );
static void state_entry_error( void );

static void state_no_process( void ); /* 処理なし */

typedef void ( *FP )( void );
static const FP state_do[ STATE_MAX ] =
{
    /* STATE_WAIT         */  &state_do_wait,
    /* STATE_WRITE        */  &state_do_write,
    /* STATE_WRITE_FINISH */  &state_no_process,
    /* STATE_READ         */  &state_do_read,
    /* STATE_READ_FINISH  */  &state_no_process,
    /* STATE_ERROR        */  &state_no_process,
};
static const FP state_exit[ STATE_MAX ] =
{
    /* STATE_WAIT         */  &state_no_process,
    /* STATE_WRITE        */  &state_exit_write,
    /* STATE_WRITE_FINISH */  &state_no_process,
    /* STATE_READ         */  &state_no_process,
    /* STATE_READ_FINISH  */  &state_no_process,
    /* STATE_ERROR        */  &state_no_process,
};
static const FP state_entry[ STATE_MAX ] =
{
    /* STATE_WAIT         */  &state_no_process,
    /* STATE_WRITE        */  &state_entry_write,
    /* STATE_WRITE_FINISH */  &state_entry_write_finish,
    /* STATE_READ         */  &state_entry_read,
    /* STATE_READ_FINISH  */  &state_entry_read_finish,
    /* STATE_ERROR        */  &state_entry_error,
};
static UBYTE state = STATE_WAIT;

void state_machine_main( void )
{
    UBYTE new;
    new = state_get_new( state );

    if( state != new )
    {
        /* 退場処理 */
        state_exit[ state ]();
        /* 入場処理 */
        state_entry[ new ]();
        state = new;
    }

    /* 定常処理 */
    state_do[ state ]();

}

static UBYTE state_get_new( UBYTE now )
{
    UBYTE   new = now;

    /* 待機指示が最優先 */
    if ( uart_get_control_data() == UART_CONTROL_WAIT_ORDER )
    {
        new = STATE_WAIT;
    }
    else
    {
        switch ( now )
        {
        case STATE_WAIT:
            /* 書き込み指示が来たら書き込み */
            if ( uart_get_control_data() == UART_CONTROL_WRITE_ORDER )
            {
                new = STATE_WRITE;
            }
            /* 読み込み指示が来たら読み込み */
            else if ( uart_get_control_data() == UART_CONTROL_READ_ORDER )
            {
                new = STATE_READ;
            }
            break;
        case STATE_WRITE:
            /* エラーありならエラーへ */
            if ( eeprom_get_is_error() != FALSE )
            {
                new = STATE_ERROR;
            }
            /* 書き込み完了 */
            else if ( eeprom_get_is_finish() != FALSE )
            {
                new = STATE_WRITE_FINISH;
            }
            break;
        case STATE_WRITE_FINISH:
            /* 待機指示が来たら待機に復帰 */
            if ( uart_get_control_data() == UART_CONTROL_WAIT_ORDER )
            {
                new = STATE_WAIT;
            }
            break;
        case STATE_READ:
            /* エラーありならエラーへ */
            if ( eeprom_get_is_error() != FALSE )
            {
                new = STATE_ERROR;
            }
            /* 読み込み完了 */
            else if ( eeprom_get_is_finish() != FALSE )
            {
                new = STATE_READ_FINISH;
            }
            break;
        case STATE_READ_FINISH:
            /* 待機指示が来たら待機に復帰 */
            if ( uart_get_control_data() == UART_CONTROL_WAIT_ORDER )
            {
                new = STATE_WAIT;
            }
            break;
        case STATE_ERROR:
            /* 待機指示が来たら待機に復帰 */
            if ( uart_get_control_data() == UART_CONTROL_WAIT_ORDER )
            {
                new = STATE_WAIT;
            }
            break;
        default:
            new = STATE_ERROR;
            break;
        }
    }

    return ( new );
}

static void state_do_wait( void )
{
    /* 送信データセット */
    uart_set_control_data( UART_CONTROL_WAIT );
}
static void state_do_write( void )
{
    /* 書き込み処理 */
    eeprom_write_main();
}
static void state_do_read( void )
{
    /* 読み込み処理 */
    eeprom_read_main();
}

static void state_exit_write( void )
{
    /* 書き込み禁止 */
    eeprom_ewds( eeprom_address_length[ uart_get_eeprom_size() ] );
}

static void state_entry_write( void )
{
    /* 初期化処理 */
    eeprom_initialize();

    /* 送信データセット */
    uart_trans_initialize( FALSE );
    uart_set_control_data( UART_CONTROL_WRITE );
    uart_set_eeprom_size( uart_get_eeprom_size() );
    uart_set_eeprom_address( uart_get_eeprom_address() );

    /* 書き込み許可 */
    eeprom_ewen( eeprom_address_length[ uart_get_eeprom_size() ] );
}
static void state_entry_read( void )
{
    /* 初期化処理 */
    eeprom_initialize();

    /* 送信データセット */
    uart_trans_initialize( FALSE );
    uart_set_control_data( UART_CONTROL_READ );
    uart_set_eeprom_size( uart_get_eeprom_size() );
    uart_set_eeprom_address( uart_get_eeprom_address() );
}
static void state_entry_write_finish( void )
{
    /* 送信データセット */
    uart_set_control_data( UART_CONTROL_WRITE_FINISH );
}
static void state_entry_read_finish( void )
{
    /* 送信データセット */
    uart_set_control_data( UART_CONTROL_READ_FINISH );
}
static void state_entry_error( void )
{
    /* 書き込み禁止 */
    eeprom_ewds( eeprom_address_length[ uart_get_eeprom_size() ] );
    
    /* UART送信データセット */
    uart_set_control_data( UART_CONTROL_ERROR );
}

static void state_no_process( void )
{
    /* DO NOTHING */
}

/* 状態を外部公開 */
UBYTE state_machine_get_state( void )
{
    return ( state );
}
