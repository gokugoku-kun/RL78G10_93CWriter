#include "define.h"

/* �I�y�R�[�h */
#define EEPROM_OPE_SIZE   (2)
#define EEPROM_OPE_EWEN   (0x00)
#define EEPROM_OPE_READ   (0x02)
#define EEPROM_OPE_WRITE  (0x01)

/* �A�h���X */
const UBYTE eeprom_address_length[ EEPROM_ADDRESS_LENGTH_SIZE ] =
{
    /* 1Kbit */  6,
    /* 2Kbit */  8,
    /* 4Kbit */  8,
    /* 8Kbit */ 10,
};
/* �f�[�^ */
#define EEPROM_DATA_SIZE  (16)
static UBYTE eeprom_data_idx_16bit = 0;

/* SK�p�^�C�} */
volatile static UBYTE eeprom_sk_wait_10us = 0;
#define EEPROM_SKCS_WAIT_10US_BEFORE (1)
#define EEPROM_SKCS_WAIT_10US_AFTER  (1)

/* �������ݑ҂��p�^�C�} */
volatile static UBYTE eeprom_write_wait_1ms = 0;
#define EEPROM_WRITE_WAIT_1MS (11)

/* 2�x��v���� */
static UDWORD eeprom_read_data_last = 0xFFFFFFFF;
#define EEPROM_READ_TRY_MAX (10)

/* �������݊����t���O */
static UBYTE eeprom_is_finish = FALSE;

/* �G���[�^�C�} */
volatile static UWORD eeprom_error_1ms = 0;
#define EEPROM_ERROR_1MS (10000)
/* �G���[�t���O */
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
            /* �������݃A�h���X���� */
            write_address = ( (UWORD)(uart_get_eeprom_address()) << (UBYTE)4 ) + (UWORD)eeprom_data_idx_16bit;
            /* �������݃f�[�^���� */
            write_data_idx_8bit_h = ( eeprom_data_idx_16bit *  2 );
            write_data_idx_8bit_l = ( eeprom_data_idx_16bit *  2 ) + 1;
            write_data_h = uart_get_eeprom_data( write_data_idx_8bit_h );
            write_data_l = uart_get_eeprom_data( write_data_idx_8bit_l );

            /* ���g���G���f�B�A���ŏ������� */
            write_data = ( (UWORD)write_data_l << (UBYTE)8 ) | (UWORD)write_data_h;

            eeprom_write( eeprom_address_length[ uart_get_eeprom_size() ], write_address , write_data );

            /* UART���M�f�[�^�Z�b�g */
            uart_set_eeprom_data( ( eeprom_data_idx_16bit * 2 )    ,  write_data_h );
            uart_set_eeprom_data( ( eeprom_data_idx_16bit * 2 ) + 1,  write_data_l );

            eeprom_data_idx_16bit = eeprom_data_idx_16bit + 1;
        }
        else
        {
            /* �������݊��� */
            eeprom_is_finish = TRUE;
        }
    }
    /* ��莞�ԓ��Ɋ������Ȃ�������G���[ */
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
            /* �ǂݍ��݃A�h���X���� */
            read_address = ( (UWORD)(uart_get_eeprom_address()) << (UBYTE)4 ) + (UWORD)eeprom_data_idx_16bit;

            /* �f�[�^�ǂݍ��� */
            {
                UBYTE read_try;
                eeprom_read_data_last = UDWORD_MAX;
                for ( read_try = 0; read_try < EEPROM_READ_TRY_MAX; read_try++ )
                {
                    read_data = eeprom_read( eeprom_address_length[ uart_get_eeprom_size() ], read_address );

                    if ( (UDWORD)read_data == eeprom_read_data_last )
                    {
                        /* �O��l�ƈ�v */
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

            /* UART���M�f�[�^�Z�b�g */
            uart_set_eeprom_data( ( eeprom_data_idx_16bit * 2 )    ,  read_data_l );
            uart_set_eeprom_data( ( eeprom_data_idx_16bit * 2 ) + 1,  read_data_h );

            eeprom_data_idx_16bit = eeprom_data_idx_16bit + 1;
        }
        else
        {
            /* �ǂݍ��݊��� */
            eeprom_is_finish = TRUE;
        }
    }
    /* ��莞�ԓ��Ɋ������Ȃ�������G���[ */
    else
    {
        if ( eeprom_is_finish == FALSE )
        {
            eeprom_is_error = TRUE;
        }
    }
}

/* �������݋��� */
void eeprom_ewen( UBYTE addres_size )
{
    UWORD address;

    eeprom_set_cs( TRUE );
    eeprom_set_sk( FALSE );

    /* �X�^�[�g�r�b�g */
    P_EEPROM_DO = TRUE;
    eeprom_set_sk( TRUE );

    /* �I�y�R�[�h */
    eeprom_ope_sub( EEPROM_OPE_SIZE, (UWORD)EEPROM_OPE_EWEN );

    /* �A�h���X */
    address = (UWORD)( 0x0003 << ( addres_size - 2 ) );
    eeprom_ope_sub( addres_size, address );

    P_EEPROM_DO = FALSE;
    eeprom_set_cs( FALSE );

}

/* �������݋֎~ */
void eeprom_ewds( UBYTE addres_size )
{
    UWORD address;

    eeprom_set_cs( TRUE );
    eeprom_set_sk( FALSE );

    /* �X�^�[�g�r�b�g */
    P_EEPROM_DO = TRUE;
    eeprom_set_sk( TRUE );

    /* �I�y�R�[�h */
    eeprom_ope_sub( EEPROM_OPE_SIZE, (UWORD)EEPROM_OPE_EWEN );

    /* �A�h���X */
    address = (UWORD)( 0x0000 << ( addres_size - 2 ) );
    eeprom_ope_sub( addres_size, address );

    P_EEPROM_DO = FALSE;
    eeprom_set_cs( FALSE );

}

/* �������� */
static void eeprom_write( UBYTE addres_size, UWORD addres, UWORD data )
{
    eeprom_set_cs( TRUE );
    eeprom_set_sk( FALSE );

    /* �X�^�[�g�r�b�g */
    P_EEPROM_DO = TRUE;
    eeprom_set_sk( TRUE );

    /* �I�y�R�[�h */
    eeprom_ope_sub( EEPROM_OPE_SIZE, (UWORD)EEPROM_OPE_WRITE );

    /* �A�h���X */
    eeprom_ope_sub( addres_size, addres );

    /* �f�[�^�������� */
    eeprom_ope_sub( EEPROM_DATA_SIZE, data );

    P_EEPROM_DO = FALSE;
    eeprom_set_cs( FALSE );
    eeprom_set_cs( TRUE );

    /* �������݊����m�F */
    {
        UBYTE is_finish = FALSE;
        eeprom_write_wait_1ms = 0;
        while ( eeprom_write_wait_1ms < EEPROM_WRITE_WAIT_1MS )
        {
            /* �O�̂��ߍŒ�ł�1msec�͑҂� */
            if ( eeprom_write_wait_1ms >= 1 )
            {
                /* �������݊���? */
                if ( P_EEPROM_DI != FALSE )
                {
                    is_finish = TRUE;
                    break;
                }
            }

        }
        /* ��莞�ԓ��Ɋ����m�F�ł��Ȃ�������G���[ */
        if ( is_finish == FALSE )
        {
            eeprom_is_error = TRUE;
        }
    }

    eeprom_set_cs( FALSE );
}

/* �ǂݍ��� */
static UWORD eeprom_read( UBYTE addres_size, UWORD addres )
{
    UWORD ret = 0;

    eeprom_set_cs( TRUE );
    eeprom_set_sk( FALSE );

    /* �X�^�[�g�r�b�g */
    P_EEPROM_DO = TRUE;
    eeprom_set_sk( TRUE );

    /* �I�y�R�[�h */
    eeprom_ope_sub( EEPROM_OPE_SIZE, (UWORD)EEPROM_OPE_READ );

    /* �A�h���X */
    eeprom_ope_sub( addres_size, addres );
    if ( P_EEPROM_DI != FALSE ) { eeprom_is_error =  TRUE; }
    P_EEPROM_DO = FALSE;

    /* �f�[�^�ǂݏo�� */
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
        /* �҂����Ԍo�߂܂ő҂�(�f�[�^�Z�b�g�A�b�v����) */
    }
    P_EEPROM_SK = data;
    eeprom_sk_wait_10us = 0;
    while ( eeprom_sk_wait_10us < EEPROM_SKCS_WAIT_10US_AFTER )
    {
        /* �҂����Ԍo�߂܂ő҂�(�o�͒x������) */
    }
}

static void eeprom_set_cs( UBYTE data )
{
    eeprom_sk_wait_10us = 0;
    while ( eeprom_sk_wait_10us < EEPROM_SKCS_WAIT_10US_BEFORE )
    {
        /* �҂����Ԍo�߂܂ő҂�(�f�[�^�Z�b�g�A�b�v����) */
    }
    P_EEPROM_CS = data;
    eeprom_sk_wait_10us = 0;
    while ( eeprom_sk_wait_10us < EEPROM_SKCS_WAIT_10US_AFTER )
    {
        /* �҂����Ԍo�߂܂ő҂�(�o�͒x������) */
    }
}

/* 10us�J�E���g�֐� */
void eeprom_sk_wait_10us_inc( void )
{
    if ( eeprom_sk_wait_10us < UBYTE_MAX )
    {
        eeprom_sk_wait_10us++;
    }
}

/* 1ms�J�E���g�֐� */
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
