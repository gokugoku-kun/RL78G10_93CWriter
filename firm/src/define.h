
#ifndef _DEFINE_H_
#define _DEFINE_H_

typedef   signed char  SBYTE;
typedef unsigned char  UBYTE;
typedef   signed short SWORD;
typedef unsigned short UWORD;
typedef   signed long  SDWORD;
typedef unsigned long  UDWORD;

#define UBYTE_MAX  (0xFF)
#define UBYTE_MIN  (0x00)
#define UWORD_MAX  (0xFFFF)
#define UWORD_MIN  (0x0000)
#define UDWORD_MAX (0xFFFFFFFF)
#define UDWORD_MIN (0x00000000)
#define SBYTE_MAX  ( 127)
#define SBYTE_MIN  (-128)
#define SWORD_MAX  ( 32767)
#define SWORD_MIN  (-32768)
#define SDWORD_MAX ( 2147483647)
#define SDWORD_MIN (-2147483648)

#define TRUE  (1)
#define FALSE (0)
#define HIGH  (1)
#define LOW   (0)

/* インクルード */
#include "iodefine.h"
#include "micon_resource.h"
#include "timer.h"
#include "uart_format.h"
#include "uart.h"
#include "eeprom.h"
#include "state_machine.h"
#include "led.h"

#endif
