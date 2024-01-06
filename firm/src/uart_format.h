#ifndef _UART_FORMAT_H_
#define _UART_FORMAT_H_

/* +-------------------------------------------------------+ */
/* |STX|control|size|address|data0|data1|...|data15|CRC|ETX| */
/* +-------------------------------------------------------+ */

enum eUART_FORMAT
{
    UART_FORMAT_CONTROL,
    UART_FORMAT_EEPROM_SIZE,
    UART_FORMAT_EEPROM_ADDRESS,
    UART_FORMAT_EEPROM_DATA,
    UART_FORMAT_MAX
};

/* �ʐM�f�[�^(���l)�̒���(BYTE�P��) */
/* ���K���e�[�u��UART_FORMAT_LENGTH[]�̍��v�l�Ƃ��邱�� */
#define UART_DATA_LENGTH_CHAR ( 35 )

/* �ʐM�t���[���f�[�^(ASCII)�̒���(BYTE�P��) */
#define UART_STX_LENGTH (1)
#define UART_CRC_LENGTH (8)
#define UART_ETX_LENGTH (1)
#define UART_DATA_LENGTH ( UART_STX_LENGTH + (UART_DATA_LENGTH_CHAR * 2) )
#define UART_FRAME_LENGTH ( UART_DATA_LENGTH + UART_CRC_LENGTH + UART_ETX_LENGTH)


extern UBYTE uart_format_get_idx( UBYTE num );
extern UBYTE uart_format_get_length( UBYTE num );


#endif
