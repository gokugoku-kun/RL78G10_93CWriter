#include "define.h"


enum eLED
{
    LED_WAIT,
    LED_READ,
    LED_WRITE,
    LED_ERROR,
    LED_MAX
};

static const UBYTE LED_SELECT[ STATE_MAX ] =
{
    LED_WAIT,   /* STATE_WAIT         */
    LED_WRITE,  /* STATE_WRITE        */
    LED_WRITE,  /* STATE_WRITE_FINISH */
    LED_READ,   /* STATE_READ         */  
    LED_READ,   /* STATE_READ_FINISH  */
    LED_ERROR,  /* STATE_ERROR        */
};

typedef struct tagPLEXA_CH
{
    UBYTE A;
    UBYTE B;
} stPLEXA_CH;

static const stPLEXA_CH PLEXA_CH[ LED_MAX ] =
{
    { 1, 1 }, /* LED_WAIT  */
    { 0, 0 }, /* LED_READ  */
    { 1, 0 }, /* LED_WRITE */
    { 0, 1 }, /* LED_ERROR */
};

void led_main( void )
{
    /* LED常時点灯 */
    P_LED = TRUE;

    /* マルチプレクサチャンネル設定 */
    P_PLEXA_A = PLEXA_CH[ LED_SELECT[ state_machine_get_state() ] ].A;
    P_PLEXA_B = PLEXA_CH[ LED_SELECT[ state_machine_get_state() ] ].B;

}