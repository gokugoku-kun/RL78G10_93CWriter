#include "define.h"

/* �ʐM�f�[�^(���l)�̊e�v�f���Ƃ̒���(BYTE�P��) */
/* �ύX������K��UART_DATA_LENGTH_CHAR���C�����邱�� */
static const UBYTE UART_FORMAT_LENGTH[ UART_FORMAT_MAX ] =
{
     1, /* UART_FORMAT_CONTROL        */
     1, /* UART_FORMAT_EEPROM_SIZE    */
     1, /* UART_FORMAT_EEPROM_ADDRESS */
    32, /* UART_FORMAT_EEPROM_DATA    */
};

UBYTE uart_format_get_idx( UBYTE num )
{
    UBYTE ret = 0;
    if ( num != 0 )
    {
        UBYTE i;
        for ( i = 0; i < num; i++ )
        {
            ret = ret + UART_FORMAT_LENGTH[ i ];
        }
    }
    return ( ret );
}

UBYTE uart_format_get_length( UBYTE num )
{
    return ( UART_FORMAT_LENGTH[ num ] );
}
