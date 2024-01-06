#include "define.h"

# define CLOCK_TYPE_IN  (0)
# define CLOCK_TYPE_OUT (1)
# define CLOCK_TYPE     (CLOCK_TYPE_IN)

void micon_resource_initialize( void );
static void micon_resource_port( void );
static void micon_resource_clock( void );
static void micon_resource_timer( void );
static void micon_resource_interrput( void );
static void micon_resource_uart0( void );

void micon_resource_initialize( void )
{
    micon_resource_port();
    micon_resource_clock();
    micon_resource_interrput();
    micon_resource_timer();
    micon_resource_uart0();
}

static void micon_resource_port( void )
{
    /* 4. 3. 1 �|�[�g�E���[�h�E���W�X�^0, 4�iPM0, PM4�j */
        PM0 = 0x02;
        PM4 = 0xFD;
    /* 4. 3. 2 �|�[�g�E���W�X�^0, 4, 12, 13�iP0, P4, P12, P13 */
        P0  = 0x01;
        P4  = 0x00;
        /* P12 = 0x00; ���͐�p */
        /* P13 = 0x00; ���͐�p */
    /* 4. 3. 3 �v���A�b�v��R�I�v�V�����E���W�X�^0, 4, 12�iPU0, PU4, PU12�j */
        PU0  = 0x00;
        PU4  = 0x00;
        PU12 = 0x20;
    /* 4. 3. 4 �|�[�g�o�̓��[�h�E���W�X�^0�iPOM0�j */
        POM0 = 0x00;
    /* 4. 3. 5 �|�[�g�E���[�h�E�R���g���[���E���W�X�^0�iPMC0�j */
        PMC0 = 0x00;
    /* 4. 3. 6 ����I/O���_�C���N�V�����E���W�X�^�iPIOR�j */
        PIOR = 0x00;
}

static void micon_resource_clock( void )
{
#if ( CLOCK_TYPE == CLOCK_TYPE_OUT )
    volatile UBYTE tmp;
    /* 5. 3. 1 �N���b�N���샂�[�h���䃌�W�X�^�iCMC�j */
        CMC = 0x41;
            /* X1���U���[�h */
            /* 10 MHz < fx <= 20 MHz */
    /*  5. 3. 5 ���U���莞�ԑI�����W�X�^�iOSTS�j*/
        OSTS = 0x07;
            /* fx = 20 MHz �� 13.1msec */
    /* 5. 3. 3 �N���b�N����X�e�[�^�X���䃌�W�X�^�iCSC�j */
        CSC = 0x01;
            /* X1���U��H���� */
    /* 5. 3. 4 ���U���莞�ԃJ�E���^��ԃ��W�X�^�iOSTC�j */
        tmp = OSTC;
        while ( tmp < 0xFF )
        {
            tmp = OSTC;
            /* OSTS�Őݒ肵�����U���莞�ԑ҂� */
        }
    /* 5. 3. 2 �V�X�e���E�N���b�N���䃌�W�X�^�iCKC�j */
        CKC = 0x10;
            /* ���C���V�X�e���N���b�Nfmain�ɍ����V�X�e���N���b�Nfmx��I�� */
        tmp = CKC_bit.no5;
        while ( CKC_bit.no5 == 0 )
        {
            tmp = CKC_bit.no5;
            /* �����V�X�e���N���b�N�ɐ؂�ւ��܂ő҂� */
        }
#else
    /* 5. 3. 1 �N���b�N���샂�[�h���䃌�W�X�^�iCMC�j */
        CMC = 0x00;
            /* ���̓|�[�g���[�h */
            /* 10 MHz < fx <= 20 MHz */
    /*  5. 3. 5 ���U���莞�ԑI�����W�X�^�iOSTS�j*/

    /* 5. 3. 3 �N���b�N����X�e�[�^�X���䃌�W�X�^�iCSC�j */
        CSC = 0x00;
            /* �����I���`�b�v�I�V���[�^���� */
    /* 5. 3. 4 ���U���莞�ԃJ�E���^��ԃ��W�X�^�iOSTC�j */

    /* 5. 3. 2 �V�X�e���E�N���b�N���䃌�W�X�^�iCKC�j */
        CKC = 0x00;
            /* ���C���V�X�e���N���b�Nfmain�ɍ����I���`�b�v�I�V���[�^fih��I�� */
#endif
    /* 5. 3. 6 ���ӃC�l�[�u���E���W�X�^0�iPER0�j */
        PER0 = 0xF5;
            /* �߂�ǂ������̂őS�Ăɋ��� */
    /* 5. 3. 7 ����X�s�[�h�E���[�h���䃌�W�X�^�iOSMC�j */
        OSMC = 0x00;
            /* 12bit�^�C�}�ɃN���b�N�������Ȃ� */
    /* 5. 3. 8 �����I���`�b�v�E�I�V���[�^���g���I�����W�X�^�iHOCODIV�j */
        /* HOCODIV = 0x01; */
            /* �����l�i�I�v�V�����o�C�g�̒l����ύX���Ȃ��j */
}

static void micon_resource_timer( void )
{
    /* 6. 3. 1 ���ӃC�l�[�u���E���W�X�^0�iPER0�j */
        /* micon_resource_clock 5.3.6 �Őݒ�ς� */
    /* 6. 3. 2 �^�C�}�E�N���b�N�I�����W�X�^0�iTPS0�j */
        TPS0 = 0x00;
            /* CK00 �������� */
            /* CK01 �������� */
    /* 6. 3. 3 �^�C�}�E���[�h�E���W�X�^0n�iTMR0n�j */
        TMR00H = 0x00;
            /* �`�����l��0����N���b�N CK00 */
            /* �`�����l��0�J�E���g�N���b�N �`�����l��0����N���b�N */
            /* �`�����l��0�g���K �\�t�g�E�F�A�g���K�X�^�[�g�̂� */
        TMR00L = 0x00;
            /* ������G�b�W */
            /* �C���^�[�o���^�C�} */
            /* �J�E���g�J�n���Ƀ^�C�}���荞�ݔ������Ȃ� */
        TMR02H = 0x00;
            /* �`�����l��2����N���b�N CK00 */
            /* �`�����l��2�J�E���g�N���b�N �`�����l��2����N���b�N */
            /* �`�����l��2�g���K �\�t�g�E�F�A�g���K�X�^�[�g�̂� */
            /* �`�����l��2�P�ƃ`���l������ */
        TMR02L = 0x00;
            /* ������G�b�W */
            /* �C���^�[�o���^�C�} */
            /* �J�E���g�J�n���Ƀ^�C�}���荞�ݔ������Ȃ� */
        TMR01H = 0x00;
            /* �`�����l��1����N���b�N CK00 */
            /* �`�����l��1�J�E���g�N���b�N �`�����l��1����N���b�N */
            /* �`�����l��1�g���K �\�t�g�E�F�A�g���K�X�^�[�g�̂� */
            /* �`�����l��1�P�ƃ`���l������ */ 
            /* �`�����l��1 16bit�^�C�}���� */
        TMR01L = 0x00;
            /* ������G�b�W */
            /* �C���^�[�o���^�C�} */
            /* �J�E���g�J�n���Ƀ^�C�}���荞�ݔ������Ȃ� */
        TMR03H = 0x00;
            /* �`�����l��3����N���b�N CK00 */
            /* �`�����l��3�J�E���g�N���b�N �`�����l��3����N���b�N */
            /* �`�����l��3�g���K �\�t�g�E�F�A�g���K�X�^�[�g�̂� */
            /* �`�����l��3�P�ƃ`���l������ */ 
            /* �`�����l��3 16bit�^�C�}���� */
        TMR03L = 0x00;
            /* ������G�b�W */
            /* �C���^�[�o���^�C�} */
            /* �J�E���g�J�n���Ƀ^�C�}���荞�ݔ������Ȃ� */
    /* 6. 2. 2 �^�C�}�E�f�[�^�E���W�X�^0n�iTDR0n�j */
        {
            UWORD count = 200 - 1; /* 10usec���� */
            TDR00H = (UBYTE)( ( count & 0xFF00 ) >> 8 );
            TDR00L = (UBYTE)( count & 0x00FF );
        }
    /*6. 3. 6 �^�C�}�E�`���l���J�n���W�X�^0�iTS0, TSH0�i8�r�b�g���[�h�j�j */
        TS0 = 0x01;
        TSH0 = 0x00;
            /* �`�����l��0 ���싖�� */
    /* 6. 3. 5 �^�C�}�E�`���l�����X�e�[�^�X�E���W�X�^�iTE0, TEH0�i8�r�b�g���[�h�j�j */
        {
            volatile UBYTE tmp;
            tmp = TE0_bit.no0;
            while ( tmp == 0 )
            {
                tmp = TE0_bit.no0;
            }
        }

}

static void micon_resource_interrput( void )
{
    /* 14. 3. 2 ���荞�݃}�X�N�E�t���O�E���W�X�^�iMK0L, MK0H, MK1L�j */
        MK0L = 0x47;
        MK0H = 0xFF;
        MK1L = 0xFF;
            /* TM00,SRE0,SR0,ST0 ���荞�݋��� */
    /* 14. 3. 3 �D�揇�ʎw��t���O�E���W�X�^�iPR00L, PR00H, PR10L, PR10H, PR01L,
PR11L�j */
        PR00L = 0x47;
        PR10L = 0xFF;
        PR00H = 0xFF;
        PR10H = 0xFF;
        PR01L = 0xFF;
        PR11L = 0xFF;
        /* TM00,SRE0,SR0,ST0 �D�揇�ʃ��x��2 */
}

static void micon_resource_uart0( void )
{
    /* 12. 3. 1 ���ӃC�l�[�u���E���W�X�^0�iPER0�j */
        /* micon_resource_clock 5.3.6 �Őݒ�ς� */
    /* 12. 3. 2 �V���A���E�N���b�N�I�����W�X�^0�iSPS0�j */
        SPS0 = 0x04;
        /* CK00��16���� */
    /* 12. 3. 3 �V���A���E���[�h�E���W�X�^0n�iSMR0nH, SMR0nL�j */
        SMR00H = 0x00;
        SMR01H = 0x01;
        /* �`�����l��0,1����N���b�N CK00 */
        /* �]���N���b�N �`�����l��0,1����N���b�N�̕����N���b�N */
        /* �\�t�g�E�F�A�g���K�̂ݗL�� */
        SMR00L = 0x23;
        SMR01L = 0x22;
        /* ���x�����] */
        /* UART���[�h */
        /* �o�b�t�@�󂫊��荞�� */
    /* 12. 3. 4 �V���A���ʐM����ݒ背�W�X�^0n�iSCR0nH, SCR0nL�j */
        SCR00H = 0x80;
        SCR01H = 0x44;
        /* �`�����l��0���M���s�� */
        /* �`�����l��1��M���s�� */
        /* �`�����l��1�G���[���荞��INTSRE���� */
        /* �p���e�B�Ȃ� */
        SCR00L = 0x97;
        SCR01L = 0x97;
        /* LSB�t�@�[�X�g */
        /* �X�g�b�v�r�b�g 1bit */
        /* �f�[�^�� 8bit */
    /* 12. 3. 5 �V���A���E�f�[�^�E���W�X�^0n�iSDR0nH, SDR0nL�j */
        SDR00H = 0x82;
        SDR01H = 0x82;
        /* ����N���b�N��130���� 9615bps*/
    /* 12. 3. 12 �V���A���o�̓��W�X�^0�iSO0�j */
        SO0 = 0x03;
        /* �����o��High���x�� */
    /* 12. 3. 14 �V���A���o�̓��x���E���W�X�^0�iSOL0�j */
        SOL0 = 0x00;
        /* ���M�f�[�^���x�����]�Ȃ� */
    /* 12. 3. 11 �V���A���o�͋����W�X�^0�iSOE0�j */
        SOE0 =0x01;
        /* �`�����l��0�V���A���o�͋��� */
    /* 12. 3. 15 �m�C�Y�E�t�B���^�����W�X�^0�iNFEN0�j */
        NFEN0 = 0x00;
        /* �m�C�Y�t�B���^�g�p���Ȃ� */
    /* 12. 3. 8 �V���A���E�`���l���J�n���W�X�^0�iSS0�j */
        SS0 = 0x03;
        /* �`�����l��0,1����J�n */
    /* 12. 3. 10 �V���A���E�`���l�����X�e�[�^�X�E���W�X�^0�iSE0�j */
        {
            volatile UBYTE tmp;
            tmp = SE0 & 0x03;
            while ( tmp != 0x03 )
            {
                tmp = SE0 & 0x03; 
            }
        }
}

void micon_resource_uart0_recieve_start( void )
{
    /* �G���[�t���O�N���A */
    uart_recieve_error_interrupt();
    /* �`�����l��1����J�n */
    SS0_bit.no1 = 1;
    {
        volatile UBYTE tmp = SE0_bit.no1;
        while ( tmp != 1 )
        {
            tmp = SE0_bit.no1;
        }
    }
}

void micon_resource_uart0_recieve_stop( void )
{
    /* �`�����l��1�����~ */
    ST0_bit.no1 = 1;
    /* �G���[�t���O�N���A */
    uart_recieve_error_interrupt();
}
