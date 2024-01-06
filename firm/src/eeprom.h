#ifndef _EEPROM_H_
#define _EEPROM_H_

#define EEPROM_ADDRESS_LENGTH_SIZE (4)
extern const UBYTE eeprom_address_length[ EEPROM_ADDRESS_LENGTH_SIZE ];

extern void eeprom_write_main( void );
extern void eeprom_read_main( void );
extern void eeprom_ewen( UBYTE addres_size );
extern void eeprom_ewds( UBYTE addres_size );
extern void eeprom_sk_wait_10us_inc( void );
extern void eeprom_wait_1ms_inc( void );
extern UBYTE eeprom_get_is_finish( void );
extern UBYTE eeprom_get_is_error( void );
extern void eeprom_initialize( void );

#endif
