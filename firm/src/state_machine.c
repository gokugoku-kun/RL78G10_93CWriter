#include "define.h"

static UBYTE state_get_new( UBYTE now );
/* ��폈�� */
static void state_do_wait( void );
static void state_do_write( void );
static void state_do_read( void );
/* �ޏꏈ�� */
static void state_exit_write( void );
/* ���ꏈ�� */
static void state_entry_write( void );
static void state_entry_read( void );
static void state_entry_write_finish( void );
static void state_entry_read_finish( void );
static void state_entry_error( void );

static void state_no_process( void ); /* �����Ȃ� */

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
        /* �ޏꏈ�� */
        state_exit[ state ]();
        /* ���ꏈ�� */
        state_entry[ new ]();
        state = new;
    }

    /* ��폈�� */
    state_do[ state ]();

}

static UBYTE state_get_new( UBYTE now )
{
    UBYTE   new = now;

    /* �ҋ@�w�����ŗD�� */
    if ( uart_get_control_data() == UART_CONTROL_WAIT_ORDER )
    {
        new = STATE_WAIT;
    }
    else
    {
        switch ( now )
        {
        case STATE_WAIT:
            /* �������ݎw���������珑������ */
            if ( uart_get_control_data() == UART_CONTROL_WRITE_ORDER )
            {
                new = STATE_WRITE;
            }
            /* �ǂݍ��ݎw����������ǂݍ��� */
            else if ( uart_get_control_data() == UART_CONTROL_READ_ORDER )
            {
                new = STATE_READ;
            }
            break;
        case STATE_WRITE:
            /* �G���[����Ȃ�G���[�� */
            if ( eeprom_get_is_error() != FALSE )
            {
                new = STATE_ERROR;
            }
            /* �������݊��� */
            else if ( eeprom_get_is_finish() != FALSE )
            {
                new = STATE_WRITE_FINISH;
            }
            break;
        case STATE_WRITE_FINISH:
            /* �ҋ@�w����������ҋ@�ɕ��A */
            if ( uart_get_control_data() == UART_CONTROL_WAIT_ORDER )
            {
                new = STATE_WAIT;
            }
            break;
        case STATE_READ:
            /* �G���[����Ȃ�G���[�� */
            if ( eeprom_get_is_error() != FALSE )
            {
                new = STATE_ERROR;
            }
            /* �ǂݍ��݊��� */
            else if ( eeprom_get_is_finish() != FALSE )
            {
                new = STATE_READ_FINISH;
            }
            break;
        case STATE_READ_FINISH:
            /* �ҋ@�w����������ҋ@�ɕ��A */
            if ( uart_get_control_data() == UART_CONTROL_WAIT_ORDER )
            {
                new = STATE_WAIT;
            }
            break;
        case STATE_ERROR:
            /* �ҋ@�w����������ҋ@�ɕ��A */
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
    /* ���M�f�[�^�Z�b�g */
    uart_set_control_data( UART_CONTROL_WAIT );
}
static void state_do_write( void )
{
    /* �������ݏ��� */
    eeprom_write_main();
}
static void state_do_read( void )
{
    /* �ǂݍ��ݏ��� */
    eeprom_read_main();
}

static void state_exit_write( void )
{
    /* �������݋֎~ */
    eeprom_ewds( eeprom_address_length[ uart_get_eeprom_size() ] );
}

static void state_entry_write( void )
{
    /* ���������� */
    eeprom_initialize();

    /* ���M�f�[�^�Z�b�g */
    uart_trans_initialize( FALSE );
    uart_set_control_data( UART_CONTROL_WRITE );
    uart_set_eeprom_size( uart_get_eeprom_size() );
    uart_set_eeprom_address( uart_get_eeprom_address() );

    /* �������݋��� */
    eeprom_ewen( eeprom_address_length[ uart_get_eeprom_size() ] );
}
static void state_entry_read( void )
{
    /* ���������� */
    eeprom_initialize();

    /* ���M�f�[�^�Z�b�g */
    uart_trans_initialize( FALSE );
    uart_set_control_data( UART_CONTROL_READ );
    uart_set_eeprom_size( uart_get_eeprom_size() );
    uart_set_eeprom_address( uart_get_eeprom_address() );
}
static void state_entry_write_finish( void )
{
    /* ���M�f�[�^�Z�b�g */
    uart_set_control_data( UART_CONTROL_WRITE_FINISH );
}
static void state_entry_read_finish( void )
{
    /* ���M�f�[�^�Z�b�g */
    uart_set_control_data( UART_CONTROL_READ_FINISH );
}
static void state_entry_error( void )
{
    /* �������݋֎~ */
    eeprom_ewds( eeprom_address_length[ uart_get_eeprom_size() ] );
    
    /* UART���M�f�[�^�Z�b�g */
    uart_set_control_data( UART_CONTROL_ERROR );
}

static void state_no_process( void )
{
    /* DO NOTHING */
}

/* ��Ԃ��O�����J */
UBYTE state_machine_get_state( void )
{
    return ( state );
}
