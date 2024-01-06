#ifndef _STATE_MACHINE_H_
#define _STATE_MACHINE_H_

enum eSTATE
{
    STATE_WAIT,
    STATE_WRITE,
    STATE_WRITE_FINISH,
    STATE_READ,
    STATE_READ_FINISH,
    STATE_ERROR,
    STATE_MAX
};

extern void state_machine_main( void );
extern UBYTE state_machine_get_state( void );

#endif
