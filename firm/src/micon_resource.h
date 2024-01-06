#ifndef _MICON_RESORCE_H_
#define _MICON_RESORCE_H_

extern void micon_resource_initialize( void );
extern void micon_resource_uart0_recieve_start( void );
extern void micon_resource_uart0_recieve_stop( void );

/* �g�p�|�[�g */
#define P_LED           ( P4_bit.no1  ) /* P41  1PIN */ /* �o�� */
#define P_40            ( P4_bit.no0  ) /* P40  2PIN */ /* ���� */
#define P_125           ( P12_bit.no5 ) /* P125 3PIN */ /* ���� */
#define P_SW            ( P13_bit.no7 ) /* P137 4PIN */ /* ���� */
#define P_EEPROM_MISO2  ( P12_bit.no2 ) /* P122 5PIN */ /* ���� */
#define P_EEPROM_MISO1  ( P12_bit.no1 ) /* P121 6PIN */ /* ���� */
                                        /* VSS  7PIN */ /* GND  */
                                        /* VDD  8PIN */ /* 5V   */
#define P_UART_TXD      ( P0_bit.no0  ) /* P00  9PIN */ /* �o�� */
#define P_UART_RXD      ( P0_bit.no1  ) /* P01 10PIN */ /* ���� */
#define P_PLEXA_A       ( P0_bit.no2  ) /* P02 11PIN */ /* �o�� */
#define P_PLEXA_B       ( P0_bit.no3  ) /* P03 12PIN */ /* �o�� */
#define P_EEPROM_CS2    ( P0_bit.no4  ) /* P04 13PIN */ /* �o�� */
#define P_EEPROM_CS1    ( P0_bit.no5  ) /* P05 14PIN */ /* �o�� */
#define P_EEPROM_SCLK   ( P0_bit.no6  ) /* P06 15PIN */ /* �o�� */
#define P_EEPROM_MOSI   ( P0_bit.no7  ) /* P07 16PIN */ /* �o�� */

#define P_EEPROM_CS ( P_EEPROM_CS1   )
#define P_EEPROM_DI ( P_EEPROM_MISO1 )
#define P_EEPROM_SK ( P_EEPROM_SCLK  )
#define P_EEPROM_DO ( P_EEPROM_MOSI  )

#endif
