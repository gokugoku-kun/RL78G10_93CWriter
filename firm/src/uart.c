#include "define.h"

static UBYTE uart_recieve_char_data[ UART_DATA_LENGTH_CHAR ];
static UBYTE uart_trans_char_data[ UART_DATA_LENGTH_CHAR ];
static UBYTE uart_recieve_frame[ UART_FRAME_LENGTH ];
static UBYTE uart_trans_frame[ UART_FRAME_LENGTH ];
static UBYTE uart_recieve_frame_idx;
static UBYTE uart_trans_frame_idx;
static UBYTE uart_trans_1ms;
static const UBYTE char_to_ascii[ 16 ] =
{
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F',
};

static UDWORD get_crc32( UBYTE *array );
static UBYTE  get_char_to_ascii( UBYTE char_data );
static UBYTE  get_ascii_to_char( UBYTE ascii_data_h, UBYTE ascii_data_l );


/************************ ��M�f�[�^�O�����J *************************/
UBYTE uart_get_control_data( void )
{
    UBYTE ret = 0;
    ret = uart_recieve_char_data[ uart_format_get_idx( UART_FORMAT_CONTROL ) ];
    return ret;
}

UBYTE uart_get_eeprom_size( void )
{
    UBYTE ret = 0;
    ret = uart_recieve_char_data[ uart_format_get_idx( UART_FORMAT_EEPROM_SIZE ) ];
    return ret;
}

UBYTE uart_get_eeprom_address( void )
{
    UBYTE ret = 0;
    ret = uart_recieve_char_data[ uart_format_get_idx( UART_FORMAT_EEPROM_ADDRESS ) ];
    return ret;
}

UBYTE uart_get_eeprom_data( UBYTE idx )
{
    UBYTE ret = 0;
    ret = uart_recieve_char_data[ uart_format_get_idx( UART_FORMAT_EEPROM_DATA ) + idx ];
    return ret;
}

/******************************* ��M ********************************/
void uart_recieve_initialize( void )
{
    {
        UBYTE i;
        for ( i = 0; i < UART_DATA_LENGTH_CHAR; i++ )
        {
            uart_recieve_char_data[ i ] = 0;
        }
        for ( i = 0; i < UART_FRAME_LENGTH; i++ )
        {
            uart_recieve_frame[ i ] = '0';
        }
    }
    uart_recieve_frame_idx = 0;
}

void uart_recieve_get_data( void )
{
    if ( uart_recieve_frame_idx >= UART_FRAME_LENGTH )
    {
        UBYTE crc_flag = TRUE;
        /* CRC�`�F�b�N */
        {
            UDWORD crc = get_crc32( uart_recieve_frame );
            UBYTE i;
            for ( i = 0; i < UART_CRC_LENGTH; i++ )
            {
                UBYTE j = 4 * ( ( -1 - i ) & 0x07 ); /* �Ȃ񂩃_�T���B�B�B�ǂ����@�͂Ȃ����H */
                UDWORD mask = ( 0xF0000000 >> ( 4 * i ) );
                if ( uart_recieve_frame[ UART_DATA_LENGTH + i ] != get_char_to_ascii( (UBYTE)( ( crc & mask ) >> j ) ) )
                {
                    crc_flag = FALSE;
                }
            }
        }

        /* CRC���v���Ă�����X�V */
        if ( crc_flag != FALSE )
        {
            /* wait���͑S�f�[�^��荞�� */
            if ( state_machine_get_state() == STATE_WAIT )
            {
                UBYTE i;
                for ( i = 0; i < UART_DATA_LENGTH_CHAR; i++ )
                {

                    UBYTE j = UART_STX_LENGTH + ( 2 * i );
                    uart_recieve_char_data[ i ] = get_ascii_to_char( uart_recieve_frame[ j ], uart_recieve_frame[ j + 1 ] );
                }
            }
            /* ����ȊO�͐���f�[�^�̂ݎ�荞�� */
            else
            {
                UBYTE i, j;
                i = uart_format_get_idx( UART_FORMAT_CONTROL );
                j = UART_STX_LENGTH + ( 2 * i );
                uart_recieve_char_data[ uart_format_get_idx( UART_FORMAT_CONTROL ) ] = get_ascii_to_char( uart_recieve_frame[ j ], uart_recieve_frame[ j + 1 ] );
            }
            
        }

        /* ��M�ĊJ */
        uart_recieve_frame_idx = 0;
        micon_resource_uart0_recieve_start();
    }
}

/* ��M���荞�� */
void uart_recieve_interrupt( void )
{
    UBYTE tmp = RXD0;
    if ( uart_recieve_frame_idx < UART_FRAME_LENGTH )
    {
        if( tmp == UART_STX_ASCII )
        {
            uart_recieve_frame_idx = 0;
            uart_recieve_frame[ uart_recieve_frame_idx ] = tmp;
            uart_recieve_frame_idx++;
        }
        else
        {
            if ( uart_recieve_frame_idx != 0 )
            {
                if ( uart_recieve_frame_idx < UART_FRAME_LENGTH )
                {
                    uart_recieve_frame[ uart_recieve_frame_idx ] = tmp;
                    uart_recieve_frame_idx++;
                }
            }
        }
    }

    if ( uart_recieve_frame_idx >= UART_FRAME_LENGTH )
    {
        /* �Ō�܂œǂ񂾂��M��~ */
        micon_resource_uart0_recieve_stop();
    }
}

/* ��M�G���[���荞�� */
void uart_recieve_error_interrupt( void )
{
    UBYTE tmp = RXD0;
    UBYTE tmp_error = SSR01 & 0x07;
    if ( tmp_error != 0 )
    {
        SIR01 = tmp_error;
    }
    if ( uart_recieve_frame_idx < UART_FRAME_LENGTH )
    {
        uart_recieve_frame_idx = 0;
    }
}

/************************ ���M�f�[�^�Z�b�g *************************/
void uart_set_control_data( UBYTE data )
{
    uart_trans_char_data[ uart_format_get_idx( UART_FORMAT_CONTROL ) ] = data;
}

void uart_set_eeprom_size( UBYTE data )
{
    uart_trans_char_data[ uart_format_get_idx( UART_FORMAT_EEPROM_SIZE ) ] = data;
}

void uart_set_eeprom_address( UBYTE data )
{
    uart_trans_char_data[ uart_format_get_idx( UART_FORMAT_EEPROM_ADDRESS ) ] = data;
}

void uart_set_eeprom_data( UBYTE idx, UBYTE data )
{
    uart_trans_char_data[ uart_format_get_idx( UART_FORMAT_EEPROM_DATA ) + idx ] = data;
}

/******************************* ���M ********************************/
void uart_trans_initialize( UBYTE is_init )
{
    UBYTE i;
    for ( i = 0; i < UART_DATA_LENGTH_CHAR; i++ )
    {
        uart_trans_char_data[ i ] = 0;
    }

    if ( is_init != FALSE )
    {
        for ( i = 0; i < UART_FRAME_LENGTH; i++ )
        {
            uart_trans_frame[ i ] = '0';
        }
        uart_trans_1ms = UART_TRANS_INTERVAL_1MS;
        uart_trans_frame_idx = 0;
    }
}

void uart_trans_set_data( void )
{
    /* �X�^�[�g�R�[�h�t�^ */
    uart_trans_frame[ 0 ] = UART_STX_ASCII;

    /* ���M�f�[�^���� */
    {
        UBYTE i;
        for ( i = UART_STX_LENGTH; i < UART_DATA_LENGTH; i++ )
        {
            UBYTE j = ( i - UART_STX_LENGTH ) / 2;
            UBYTE l = ( i - UART_STX_LENGTH ) & 0x01;
            if ( l == 0 )
            {
                uart_trans_frame[ i ] = get_char_to_ascii( ( uart_trans_char_data[ j ] & 0xF0 ) >> 4 );
            }
            else
            {
                uart_trans_frame[ i ] = get_char_to_ascii( uart_trans_char_data[ j ] & 0x0F );
            }
        }
    }

    /* CRC32�t�^ */
    {
        UDWORD crc = get_crc32( uart_trans_frame );
        UBYTE i;
        for ( i = 0; i < UART_CRC_LENGTH; i++ )
        {
            UBYTE j = 4 * ( ( -1 - i ) & 0x07 ); /* �Ȃ񂩃_�T���B�B�B�ǂ����@�͂Ȃ����H */
            UDWORD mask = ( 0xF0000000 >> ( 4 * i ) );
            uart_trans_frame[ UART_DATA_LENGTH + i ] = get_char_to_ascii( (UBYTE)( ( crc & mask ) >> j ) );
        }
    }

    /* �G���h�R�[�h�t�^ */
    uart_trans_frame[ UART_FRAME_LENGTH - 1 ] = UART_ETX_ASCII;

    /* �C���^�[�o���o�߂�����X�^�[�g�R�[�h���瑗�M�J�n */
    if ( uart_trans_1ms >= UART_TRANS_INTERVAL_1MS )
    {
        uart_trans_frame_idx = 0;
        TXD0 = uart_trans_frame[ uart_trans_frame_idx ];
        uart_trans_1ms = 0;
    }
}

/* ���M�o�b�t�@�G���v�e�B���荞�� */
void uart_trans_interrupt( void )
{
    if ( uart_trans_frame_idx < UBYTE_MAX )
    {
        uart_trans_frame_idx++;
    }

    if ( uart_trans_frame_idx < UART_FRAME_LENGTH )
    {
        TXD0 = uart_trans_frame[ uart_trans_frame_idx ];
        /* ���M���̓^�C�}�N���A */
        uart_trans_1ms = 0;
    }
}

/* 1ms���Ɏ��s���邱�� */
void uart_trans_1ms_inc( void )
{
    if( uart_trans_1ms < UBYTE_MAX )
    {
        uart_trans_1ms++;
    }
}

static UDWORD get_crc32( UBYTE *array ) /* �v�Z�@���Ԉ���Ă���ۂ��B����M�œ����v�Z�@�Ȃ�ǂ��̂Ŏ�肠�������V */
{
    #define CRC32 (0x04C11DB7)
    UDWORD crc = 0xFFFFFFFF;
    UBYTE i;
    for ( i = UART_STX_LENGTH; i < UART_DATA_LENGTH; i++ )
    {
        UBYTE k;
        for ( k = 0; k < 8; k++ )
        {
            UDWORD msb = ( crc & 0x80000000 );
            /* crc��1bit���V�t�g */
            { 
                crc = ( crc << 1 );
                if ( ( array[ i ] << k ) & 0x80 )
                {
                    crc |= (UDWORD)1;
                }
            }
            /* ��ꂽbit��1�Ȃ�XOR */
            if ( msb != 0 )
            {
                crc ^= CRC32;
            }
        }
    }
    return ( crc ^ 0xFFFFFFFF );
}

static UBYTE get_char_to_ascii( UBYTE char_data )
{
    UBYTE ascii_data = '0';

    if ( char_data <= 0x0F )
    {
        ascii_data = char_to_ascii[ char_data ];
    }

    return ( ascii_data );
}

static UBYTE  get_ascii_to_char( UBYTE ascii_data_h, UBYTE ascii_data_l )
{
    UBYTE char_data = 0;
    UBYTE char_data_h = 0;
    UBYTE char_data_l = 0;
    UBYTE i;

    for ( i = 0; i < 16; i++ )
    {
        if ( ascii_data_h == char_to_ascii[ i ] )
        {
            char_data_h = i;
        }
        if ( ascii_data_l == char_to_ascii[ i ] )
        {
            char_data_l = i;
        }
    }

    char_data = ( ( char_data_h << 4 ) | char_data_l );

    return ( char_data );
}
