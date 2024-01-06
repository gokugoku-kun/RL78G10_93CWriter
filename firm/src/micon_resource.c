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
    /* 4. 3. 1 ポート・モード・レジスタ0, 4（PM0, PM4） */
        PM0 = 0x02;
        PM4 = 0xFD;
    /* 4. 3. 2 ポート・レジスタ0, 4, 12, 13（P0, P4, P12, P13 */
        P0  = 0x01;
        P4  = 0x00;
        /* P12 = 0x00; 入力専用 */
        /* P13 = 0x00; 入力専用 */
    /* 4. 3. 3 プルアップ抵抗オプション・レジスタ0, 4, 12（PU0, PU4, PU12） */
        PU0  = 0x00;
        PU4  = 0x00;
        PU12 = 0x20;
    /* 4. 3. 4 ポート出力モード・レジスタ0（POM0） */
        POM0 = 0x00;
    /* 4. 3. 5 ポート・モード・コントロール・レジスタ0（PMC0） */
        PMC0 = 0x00;
    /* 4. 3. 6 周辺I/Oリダイレクション・レジスタ（PIOR） */
        PIOR = 0x00;
}

static void micon_resource_clock( void )
{
#if ( CLOCK_TYPE == CLOCK_TYPE_OUT )
    volatile UBYTE tmp;
    /* 5. 3. 1 クロック動作モード制御レジスタ（CMC） */
        CMC = 0x41;
            /* X1発振モード */
            /* 10 MHz < fx <= 20 MHz */
    /*  5. 3. 5 発振安定時間選択レジスタ（OSTS）*/
        OSTS = 0x07;
            /* fx = 20 MHz 時 13.1msec */
    /* 5. 3. 3 クロック動作ステータス制御レジスタ（CSC） */
        CSC = 0x01;
            /* X1発振回路動作 */
    /* 5. 3. 4 発振安定時間カウンタ状態レジスタ（OSTC） */
        tmp = OSTC;
        while ( tmp < 0xFF )
        {
            tmp = OSTC;
            /* OSTSで設定した発振安定時間待つ */
        }
    /* 5. 3. 2 システム・クロック制御レジスタ（CKC） */
        CKC = 0x10;
            /* メインシステムクロックfmainに高速システムクロックfmxを選択 */
        tmp = CKC_bit.no5;
        while ( CKC_bit.no5 == 0 )
        {
            tmp = CKC_bit.no5;
            /* 高速システムクロックに切り替わるまで待つ */
        }
#else
    /* 5. 3. 1 クロック動作モード制御レジスタ（CMC） */
        CMC = 0x00;
            /* 入力ポートモード */
            /* 10 MHz < fx <= 20 MHz */
    /*  5. 3. 5 発振安定時間選択レジスタ（OSTS）*/

    /* 5. 3. 3 クロック動作ステータス制御レジスタ（CSC） */
        CSC = 0x00;
            /* 高速オンチップオシレータ動作 */
    /* 5. 3. 4 発振安定時間カウンタ状態レジスタ（OSTC） */

    /* 5. 3. 2 システム・クロック制御レジスタ（CKC） */
        CKC = 0x00;
            /* メインシステムクロックfmainに高速オンチップオシレータfihを選択 */
#endif
    /* 5. 3. 6 周辺イネーブル・レジスタ0（PER0） */
        PER0 = 0xF5;
            /* めんどくさいので全てに供給 */
    /* 5. 3. 7 動作スピード・モード制御レジスタ（OSMC） */
        OSMC = 0x00;
            /* 12bitタイマにクロック供給しない */
    /* 5. 3. 8 高速オンチップ・オシレータ周波数選択レジスタ（HOCODIV） */
        /* HOCODIV = 0x01; */
            /* 初期値（オプションバイトの値から変更しない） */
}

static void micon_resource_timer( void )
{
    /* 6. 3. 1 周辺イネーブル・レジスタ0（PER0） */
        /* micon_resource_clock 5.3.6 で設定済み */
    /* 6. 3. 2 タイマ・クロック選択レジスタ0（TPS0） */
        TPS0 = 0x00;
            /* CK00 分周無し */
            /* CK01 分周無し */
    /* 6. 3. 3 タイマ・モード・レジスタ0n（TMR0n） */
        TMR00H = 0x00;
            /* チャンネル0動作クロック CK00 */
            /* チャンネル0カウントクロック チャンネル0動作クロック */
            /* チャンネル0トリガ ソフトウェアトリガスタートのみ */
        TMR00L = 0x00;
            /* 立下りエッジ */
            /* インターバルタイマ */
            /* カウント開始時にタイマ割り込み発生しない */
        TMR02H = 0x00;
            /* チャンネル2動作クロック CK00 */
            /* チャンネル2カウントクロック チャンネル2動作クロック */
            /* チャンネル2トリガ ソフトウェアトリガスタートのみ */
            /* チャンネル2単独チャネル動作 */
        TMR02L = 0x00;
            /* 立下りエッジ */
            /* インターバルタイマ */
            /* カウント開始時にタイマ割り込み発生しない */
        TMR01H = 0x00;
            /* チャンネル1動作クロック CK00 */
            /* チャンネル1カウントクロック チャンネル1動作クロック */
            /* チャンネル1トリガ ソフトウェアトリガスタートのみ */
            /* チャンネル1単独チャネル動作 */ 
            /* チャンネル1 16bitタイマ動作 */
        TMR01L = 0x00;
            /* 立下りエッジ */
            /* インターバルタイマ */
            /* カウント開始時にタイマ割り込み発生しない */
        TMR03H = 0x00;
            /* チャンネル3動作クロック CK00 */
            /* チャンネル3カウントクロック チャンネル3動作クロック */
            /* チャンネル3トリガ ソフトウェアトリガスタートのみ */
            /* チャンネル3単独チャネル動作 */ 
            /* チャンネル3 16bitタイマ動作 */
        TMR03L = 0x00;
            /* 立下りエッジ */
            /* インターバルタイマ */
            /* カウント開始時にタイマ割り込み発生しない */
    /* 6. 2. 2 タイマ・データ・レジスタ0n（TDR0n） */
        {
            UWORD count = 200 - 1; /* 10usec相当 */
            TDR00H = (UBYTE)( ( count & 0xFF00 ) >> 8 );
            TDR00L = (UBYTE)( count & 0x00FF );
        }
    /*6. 3. 6 タイマ・チャネル開始レジスタ0（TS0, TSH0（8ビットモード）） */
        TS0 = 0x01;
        TSH0 = 0x00;
            /* チャンネル0 動作許可 */
    /* 6. 3. 5 タイマ・チャネル許可ステータス・レジスタ（TE0, TEH0（8ビットモード）） */
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
    /* 14. 3. 2 割り込みマスク・フラグ・レジスタ（MK0L, MK0H, MK1L） */
        MK0L = 0x47;
        MK0H = 0xFF;
        MK1L = 0xFF;
            /* TM00,SRE0,SR0,ST0 割り込み許可 */
    /* 14. 3. 3 優先順位指定フラグ・レジスタ（PR00L, PR00H, PR10L, PR10H, PR01L,
PR11L） */
        PR00L = 0x47;
        PR10L = 0xFF;
        PR00H = 0xFF;
        PR10H = 0xFF;
        PR01L = 0xFF;
        PR11L = 0xFF;
        /* TM00,SRE0,SR0,ST0 優先順位レベル2 */
}

static void micon_resource_uart0( void )
{
    /* 12. 3. 1 周辺イネーブル・レジスタ0（PER0） */
        /* micon_resource_clock 5.3.6 で設定済み */
    /* 12. 3. 2 シリアル・クロック選択レジスタ0（SPS0） */
        SPS0 = 0x04;
        /* CK00を16分周 */
    /* 12. 3. 3 シリアル・モード・レジスタ0n（SMR0nH, SMR0nL） */
        SMR00H = 0x00;
        SMR01H = 0x01;
        /* チャンネル0,1動作クロック CK00 */
        /* 転送クロック チャンネル0,1動作クロックの分周クロック */
        /* ソフトウェアトリガのみ有効 */
        SMR00L = 0x23;
        SMR01L = 0x22;
        /* レベル正転 */
        /* UARTモード */
        /* バッファ空き割り込み */
    /* 12. 3. 4 シリアル通信動作設定レジスタ0n（SCR0nH, SCR0nL） */
        SCR00H = 0x80;
        SCR01H = 0x44;
        /* チャンネル0送信を行う */
        /* チャンネル1受信を行う */
        /* チャンネル1エラー割り込みINTSRE許可 */
        /* パリティなし */
        SCR00L = 0x97;
        SCR01L = 0x97;
        /* LSBファースト */
        /* ストップビット 1bit */
        /* データ長 8bit */
    /* 12. 3. 5 シリアル・データ・レジスタ0n（SDR0nH, SDR0nL） */
        SDR00H = 0x82;
        SDR01H = 0x82;
        /* 動作クロックを130分周 9615bps*/
    /* 12. 3. 12 シリアル出力レジスタ0（SO0） */
        SO0 = 0x03;
        /* 初期出力Highレベル */
    /* 12. 3. 14 シリアル出力レベル・レジスタ0（SOL0） */
        SOL0 = 0x00;
        /* 送信データレベル反転なし */
    /* 12. 3. 11 シリアル出力許可レジスタ0（SOE0） */
        SOE0 =0x01;
        /* チャンネル0シリアル出力許可 */
    /* 12. 3. 15 ノイズ・フィルタ許可レジスタ0（NFEN0） */
        NFEN0 = 0x00;
        /* ノイズフィルタ使用しない */
    /* 12. 3. 8 シリアル・チャネル開始レジスタ0（SS0） */
        SS0 = 0x03;
        /* チャンネル0,1動作開始 */
    /* 12. 3. 10 シリアル・チャネル許可ステータス・レジスタ0（SE0） */
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
    /* エラーフラグクリア */
    uart_recieve_error_interrupt();
    /* チャンネル1動作開始 */
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
    /* チャンネル1動作停止 */
    ST0_bit.no1 = 1;
    /* エラーフラグクリア */
    uart_recieve_error_interrupt();
}
