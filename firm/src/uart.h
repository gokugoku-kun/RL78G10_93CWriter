#ifndef _UART_H_
#define _UART_H_

#define UART_STX_ASCII (0x02)
#define UART_ETX_ASCII (0x03)

/* 送信フレームインターバル */
#define UART_TRANS_INTERVAL_1MS (10)

/* 制御データ */
enum eUART_CONTROL
{
    UART_CONTROL_INVALID,
    UART_CONTROL_WAIT,
    UART_CONTROL_WRITE_ORDER,
    UART_CONTROL_WRITE,
    UART_CONTROL_WRITE_FINISH,
    UART_CONTROL_READ_ORDER,
    UART_CONTROL_READ,
    UART_CONTROL_READ_FINISH,
    UART_CONTROL_ERROR,
    UART_CONTROL_ERASEALL_ORDER,    /* 未対応 */
    UART_CONTROL_ERASEALL,          /* 未対応 */
    UART_CONTROL_ERASEALL_FINISH,   /* 未対応 */
    UART_CONTROL_MAX,
    UART_CONTROL_WAIT_ORDER = 0xFF,
};

/************************ 受信データ外部公開 *************************/
extern UBYTE uart_get_control_data( void );
extern UBYTE uart_get_eeprom_size( void );
extern UBYTE uart_get_eeprom_address( void );
extern UBYTE uart_get_eeprom_data( UBYTE idx );

/******************************* 受信 ********************************/
extern void uart_recieve_initialize( void );
extern void uart_recieve_get_data( void );
extern void uart_recieve_interrupt( void );
extern void uart_recieve_error_interrupt( void );

/************************ 送信データセット *************************/
extern void uart_set_control_data( UBYTE data );
extern void uart_set_eeprom_size( UBYTE data );
extern void uart_set_eeprom_address( UBYTE data );
extern void uart_set_eeprom_data( UBYTE idx, UBYTE data );

/******************************* 送信 ********************************/
extern void uart_trans_initialize( UBYTE is_init );
extern void uart_trans_set_data( void );
extern void uart_trans_interrupt( void );
extern void uart_trans_1ms_inc( void );

#endif
