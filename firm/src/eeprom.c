#include "define.h"

/* オペコード */
#define EEPROM_OPE_SIZE   (2)
#define EEPROM_OPE_EWEN   (0x00)
#define EEPROM_OPE_READ   (0x02)
#define EEPROM_OPE_WRITE  (0x01)

/* アドレス */
const UBYTE eeprom_address_length[ EEPROM_ADDRESS_LENGTH_SIZE ] =
{
    /* 1Kbit */  6,
    /* 2Kbit */  8,
    /* 4Kbit */  8,
    /* 8Kbit */ 10,
};
/* データ */
#define EEPROM_DATA_SIZE  (16)
static UBYTE eeprom_data_idx_16bit = 0;

/* SK用タイマ */
volatile static UBYTE eeprom_sk_wait_10us = 0;
#define EEPROM_SKCS_WAIT_10US_BEFORE (1)
#define EEPROM_SKCS_WAIT_10US_AFTER  (1)

/* 書き込み待ち用タイマ */
volatile static UBYTE eeprom_write_wait_1ms = 0;
#define EEPROM_WRITE_WAIT_1MS (11)

/* 2度一致検査 */
static UDWORD eeprom_read_data_last = 0xFFFFFFFF;
#define EEPROM_READ_TRY_MAX (10)

/* 書き込み完了フラグ */
static UBYTE eeprom_is_finish = FALSE;

/* エラータイマ */
volatile static UWORD eeprom_error_1ms = 0;
#define EEPROM_ERROR_1MS (10000)
/* エラーフラグ */
static UBYTE eeprom_is_error = FALSE;

static void eeprom_write( UBYTE addres_size, UWORD addres, UWORD data );
static UWORD eeprom_read( UBYTE addres_size, UWORD addres );
static void eeprom_ope_sub( UBYTE size, UWORD data );
static void eeprom_set_sk( UBYTE data );
static void eeprom_set_cs( UBYTE data );

void eeprom_write_main( void )
{
    UWORD write_address;
    UBYTE write_data_idx_8bit_h;
    UBYTE write_data_idx_8bit_l;
    UBYTE write_data_h;
    UBYTE write_data_l;
    UWORD write_data;

    if ( eeprom_error_1ms < EEPROM_ERROR_1MS )
    {
        if ( eeprom_data_idx_16bit < ( uart_format_get_length( UART_FORMAT_EEPROM_DATA ) / 2 ) )
        {
            /* 書き込みアドレス決定 */
            write_address = ( (UWORD)(uart_get_eeprom_address()) << (UBYTE)4 ) + (UWORD)eeprom_data_idx_16bit;
            /* 書き込みデータ決定 */
            write_data_idx_8bit_h = ( eeprom_data_idx_16bit *  2 );
            write_data_idx_8bit_l = ( eeprom_data_idx_16bit *  2 ) + 1;
            write_data_h = uart_get_eeprom_data( write_data_idx_8bit_h );
            write_data_l = uart_get_eeprom_data( write_data_idx_8bit_l );

            /* リトルエンディアンで書き込む */
            write_data = ( (UWORD)write_data_l << (UBYTE)8 ) | (UWORD)write_data_h;

            eeprom_write( eeprom_address_length[ uart_get_eeprom_size() ], write_address , write_data );

            /* UART送信データセット */
            uart_set_eeprom_data( ( eeprom_data_idx_16bit * 2 )    ,  write_data_h );
            uart_set_eeprom_data( ( eeprom_data_idx_16bit * 2 ) + 1,  write_data_l );

            eeprom_data_idx_16bit = eeprom_data_idx_16bit + 1;
        }
        else
        {
            /* 書き込み完了 */
            eeprom_is_finish = TRUE;
        }
    }
    /* 一定時間内に完了しなかったらエラー */
    else
    {
        if ( eeprom_is_finish == FALSE )
        {
            eeprom_is_error = TRUE;
        }
    }
    
}

void eeprom_read_main( void )
{
    UWORD read_address;
    UWORD read_data;
    UBYTE read_data_h;
    UBYTE read_data_l;
    if ( eeprom_error_1ms < EEPROM_ERROR_1MS )
    {
        if ( eeprom_data_idx_16bit < ( uart_format_get_length( UART_FORMAT_EEPROM_DATA ) / 2 ) )
        {
            /* 読み込みアドレス決定 */
            read_address = ( (UWORD)(uart_get_eeprom_address()) << (UBYTE)4 ) + (UWORD)eeprom_data_idx_16bit;

            /* データ読み込み */
            {
                UBYTE read_try;
                eeprom_read_data_last = UDWORD_MAX;
                for ( read_try = 0; read_try < EEPROM_READ_TRY_MAX; read_try++ )
                {
                    read_data = eeprom_read( eeprom_address_length[ uart_get_eeprom_size() ], read_address );

                    if ( (UDWORD)read_data == eeprom_read_data_last )
                    {
                        /* 前回値と一致 */
                        break;
                    }
                    else
                    {
                        /* DO NOTHING */
                    }

                    eeprom_read_data_last = (UDWORD)read_data;
                }
                if ( read_try ==  EEPROM_READ_TRY_MAX )
                {
                    eeprom_is_error = TRUE;
                }
            }

            read_data_h = ( read_data >> (UBYTE)8 );
            read_data_l = (UBYTE)( read_data & 0xFF );

            /* UART送信データセット */
            uart_set_eeprom_data( ( eeprom_data_idx_16bit * 2 )    ,  read_data_l );
            uart_set_eeprom_data( ( eeprom_data_idx_16bit * 2 ) + 1,  read_data_h );

            eeprom_data_idx_16bit = eeprom_data_idx_16bit + 1;
        }
        else
        {
            /* 読み込み完了 */
            eeprom_is_finish = TRUE;
        }
    }
    /* 一定時間内に完了しなかったらエラー */
    else
    {
        if ( eeprom_is_finish == FALSE )
        {
            eeprom_is_error = TRUE;
        }
    }
}

/* 書き込み許可 */
void eeprom_ewen( UBYTE addres_size )
{
    UWORD address;

    eeprom_set_cs( TRUE );
    eeprom_set_sk( FALSE );

    /* スタートビット */
    P_EEPROM_DO = TRUE;
    eeprom_set_sk( TRUE );

    /* オペコード */
    eeprom_ope_sub( EEPROM_OPE_SIZE, (UWORD)EEPROM_OPE_EWEN );

    /* アドレス */
    address = (UWORD)( 0x0003 << ( addres_size - 2 ) );
    eeprom_ope_sub( addres_size, address );

    P_EEPROM_DO = FALSE;
    eeprom_set_cs( FALSE );

}

/* 書き込み禁止 */
void eeprom_ewds( UBYTE addres_size )
{
    UWORD address;

    eeprom_set_cs( TRUE );
    eeprom_set_sk( FALSE );

    /* スタートビット */
    P_EEPROM_DO = TRUE;
    eeprom_set_sk( TRUE );

    /* オペコード */
    eeprom_ope_sub( EEPROM_OPE_SIZE, (UWORD)EEPROM_OPE_EWEN );

    /* アドレス */
    address = (UWORD)( 0x0000 << ( addres_size - 2 ) );
    eeprom_ope_sub( addres_size, address );

    P_EEPROM_DO = FALSE;
    eeprom_set_cs( FALSE );

}

/* 書き込み */
static void eeprom_write( UBYTE addres_size, UWORD addres, UWORD data )
{
    eeprom_set_cs( TRUE );
    eeprom_set_sk( FALSE );

    /* スタートビット */
    P_EEPROM_DO = TRUE;
    eeprom_set_sk( TRUE );

    /* オペコード */
    eeprom_ope_sub( EEPROM_OPE_SIZE, (UWORD)EEPROM_OPE_WRITE );

    /* アドレス */
    eeprom_ope_sub( addres_size, addres );

    /* データ書き込み */
    eeprom_ope_sub( EEPROM_DATA_SIZE, data );

    P_EEPROM_DO = FALSE;
    eeprom_set_cs( FALSE );
    eeprom_set_cs( TRUE );

    /* 書き込み完了確認 */
    {
        UBYTE is_finish = FALSE;
        eeprom_write_wait_1ms = 0;
        while ( eeprom_write_wait_1ms < EEPROM_WRITE_WAIT_1MS )
        {
            /* 念のため最低でも1msecは待つ */
            if ( eeprom_write_wait_1ms >= 1 )
            {
                /* 書き込み完了? */
                if ( P_EEPROM_DI != FALSE )
                {
                    is_finish = TRUE;
                    break;
                }
            }

        }
        /* 一定時間内に完了確認できなかったらエラー */
        if ( is_finish == FALSE )
        {
            eeprom_is_error = TRUE;
        }
    }

    eeprom_set_cs( FALSE );
}

/* 読み込み */
static UWORD eeprom_read( UBYTE addres_size, UWORD addres )
{
    UWORD ret = 0;

    eeprom_set_cs( TRUE );
    eeprom_set_sk( FALSE );

    /* スタートビット */
    P_EEPROM_DO = TRUE;
    eeprom_set_sk( TRUE );

    /* オペコード */
    eeprom_ope_sub( EEPROM_OPE_SIZE, (UWORD)EEPROM_OPE_READ );

    /* アドレス */
    eeprom_ope_sub( addres_size, addres );
    if ( P_EEPROM_DI != FALSE ) { eeprom_is_error =  TRUE; }
    P_EEPROM_DO = FALSE;

    /* データ読み出し */
    {
        UBYTE i = 0;
        for ( i = 0; i < EEPROM_DATA_SIZE; i++ )
        {
            UBYTE bit = 0;
            eeprom_set_sk( FALSE );
            eeprom_set_sk( TRUE );

            ret = ret << 1;
            bit = P_EEPROM_DI;
            if ( bit != FALSE )
            {
                ret |= 0x0001;
            }
        }
    }

    P_EEPROM_DO = FALSE;
    eeprom_set_cs( FALSE );
    
    return ret;
}

static void eeprom_ope_sub( UBYTE size, UWORD data )
{
    UBYTE i = 0;
    for ( i = 0; i < size; i++ )
    {
        UBYTE bit = 0;
        eeprom_set_sk( FALSE );
        bit = (UBYTE)( (data >> (size-(1+i))) & 0x0001 );
        P_EEPROM_DO = bit;
        eeprom_set_sk( TRUE );
    }
}

static void eeprom_set_sk( UBYTE data )
{
    eeprom_sk_wait_10us = 0;
    while ( eeprom_sk_wait_10us < EEPROM_SKCS_WAIT_10US_BEFORE )
    {
        /* 待ち時間経過まで待つ(データセットアップ時間) */
    }
    P_EEPROM_SK = data;
    eeprom_sk_wait_10us = 0;
    while ( eeprom_sk_wait_10us < EEPROM_SKCS_WAIT_10US_AFTER )
    {
        /* 待ち時間経過まで待つ(出力遅延時間) */
    }
}

static void eeprom_set_cs( UBYTE data )
{
    eeprom_sk_wait_10us = 0;
    while ( eeprom_sk_wait_10us < EEPROM_SKCS_WAIT_10US_BEFORE )
    {
        /* 待ち時間経過まで待つ(データセットアップ時間) */
    }
    P_EEPROM_CS = data;
    eeprom_sk_wait_10us = 0;
    while ( eeprom_sk_wait_10us < EEPROM_SKCS_WAIT_10US_AFTER )
    {
        /* 待ち時間経過まで待つ(出力遅延時間) */
    }
}

/* 10usカウント関数 */
void eeprom_sk_wait_10us_inc( void )
{
    if ( eeprom_sk_wait_10us < UBYTE_MAX )
    {
        eeprom_sk_wait_10us++;
    }
}

/* 1msカウント関数 */
void eeprom_wait_1ms_inc( void )
{
    if ( eeprom_write_wait_1ms < UBYTE_MAX )
    {
        eeprom_write_wait_1ms++;
    }
    if ( eeprom_error_1ms < UWORD_MAX )
    {
        eeprom_error_1ms++;
    }
}

UBYTE eeprom_get_is_finish( void )
{
    return ( eeprom_is_finish );
}

UBYTE eeprom_get_is_error( void )
{
    return ( eeprom_is_error );
}

void eeprom_initialize( void )
{
    eeprom_is_finish = FALSE;
    eeprom_data_idx_16bit = 0;
    eeprom_error_1ms = 0;
    eeprom_is_error = 0;
}
