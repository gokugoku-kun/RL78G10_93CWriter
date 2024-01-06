using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using System.IO.Ports;
using System.Windows.Forms;

namespace rl78g10_eeprom
{
    public partial class Form1 : Form
    {

    public struct stEEP_PARAM
    {
        public string size_st;
        public byte size_id;
        public int idx_max; 
    }

    public stEEP_PARAM[] EEP_PARAM =
    {
       new stEEP_PARAM {size_st = "1Kbit", size_id = 0,  idx_max = 4 },
       new stEEP_PARAM {size_st = "2Kbit", size_id = 1,  idx_max = 8 },
       new stEEP_PARAM {size_st = "4Kbit", size_id = 2,  idx_max = 16 },
       new stEEP_PARAM {size_st = "8Kbit", size_id = 3,  idx_max = 32 }
    };
    private 
        /* 制御データ */
        enum E_UART_CONTROL:byte
    {
        UART_CONTROL_INVALID,
        UART_CONTROL_WAIT,
        UART_CONTROL_WRITE_ORDER,
        UART_CONTROL_WRITE,
        UART_CONTROL_WRITE_FINISH,
        UART_CONTROL_READ_ORDER,
        UART_CONTROL_READ,
        UART_CONTROL_READ_FINISH,
        UART_CONTROL_ERROR,
        UART_CONTROL_ERASEALL_ORDER,    /* 未対応 */
        UART_CONTROL_ERASEALL,          /* 未対応 */
        UART_CONTROL_ERASEALL_FINISH,   /* 未対応 */
        UART_CONTROL_MAX,
        UART_CONTROL_WAIT_ORDER = 0xFF,
    };
    private string[] ascii = new string[16] { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F" };
    private int form1PageNo = 0;//ページ切替
    private string[] form1CsvData = Enumerable.Repeat<string>("xx", 16*4*16).ToArray();
    private string[] EepromData   = Enumerable.Repeat<string>("xx", 16*4*16).ToArray();
    private static uint uart_trans_time_ms = 0;
    private byte eeprom_idx = 0;
    private static byte is_read = 0;
    private static byte is_write = 0;
    private static byte read_idx = (byte)0;
    private static byte write_idx = (byte)0;
    private static byte is_error_clear = 0;

        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            string[] ports = SerialPort.GetPortNames();
            this.toolStripConmboBox_Connect_Com.Items.Clear();
            foreach ( string port in ports )
            {
                this.toolStripConmboBox_Connect_Com.Items.Add(port);
            }
            if ( this.toolStripConmboBox_Connect_Com.Items.Count > 0)
            {
                this.toolStripConmboBox_Connect_Com.SelectedIndex = 0;
            }

            foreach ( stEEP_PARAM eep_param in EEP_PARAM )
            {
                this.toolStripConmboBox_Connect_EepSize.Items.Add(eep_param.size_st);
            }
            this.toolStripConmboBox_Connect_EepSize.SelectedIndex = 0;

            this.timer.Start();
        }

        private void toolStripMenuItem_Connect_Connect_Click(object sender, EventArgs e)
        {
            this.serialPort1.Close();
            this.serialPort1.BaudRate = 9615;
            this.serialPort1.Parity = Parity.None;
            this.serialPort1.DataBits = 8;
            this.serialPort1.StopBits = StopBits.One;
            this.serialPort1.Handshake = Handshake.None;
            this.serialPort1.PortName = toolStripConmboBox_Connect_Com.SelectedItem.ToString();
                
            this.serialPort1.Open();

            this.eeprom_idx = (byte)EEP_PARAM[this.toolStripConmboBox_Connect_EepSize.SelectedIndex].size_id;
        }

        private void toolStripMenuItem_Connect_Disconnect_Click(object sender, EventArgs e)
        {
            this.serialPort1.Close();
        }

        private void toolStripMenuItem_Connect_Com_Click(object sender, EventArgs e)
        {
            string[] ports = SerialPort.GetPortNames();
            this.toolStripConmboBox_Connect_Com.Items.Clear();
            foreach ( string port in ports )
            {
                this.toolStripConmboBox_Connect_Com.Items.Add(port);
            }
            if ( this.toolStripConmboBox_Connect_Com.Items.Count > 0)
            {
                this.toolStripConmboBox_Connect_Com.SelectedIndex = 0;
            }
        }
        private void toolStripMenuItem_File_CsvInput_Click(object sender, EventArgs e)
        {
            DialogResult dr = openFileDialog1.ShowDialog();
            if (dr == System.Windows.Forms.DialogResult.OK)
            {
                string[] strData;
                string strLine;
                byte tmp;
                StreamReader sr = new StreamReader(openFileDialog1.FileName, Encoding.Default);

                strLine = sr.ReadLine();//1行目を読み捨てる
                for ( int i = 0; i < 16*4; i++ )
                {
                    strLine = sr.ReadLine();
                    strData = strLine.Split(",".ToCharArray());
                    for ( int j = 0; j < 16; j++ )
                    {
                        try
                        {
                            tmp = Convert.ToByte(strData[j + 1], 16);
                            if ((tmp >= 0) && (tmp <= 255))
                            {
                                this.EepromData[i * 16 + j] = strData[j + 1];
                            }
                        }
                        catch
                        {
                            /* DO NOTING */
                        }
                    }
                }
                for (int i = 0; i < this.textBoxArray.Length; i++)
                {
                    this.textBoxArray[i].Text = this.EepromData[i + this.form1PageNo * 16 * 2 * 16];
                }
                sr.Close();
            }
        }
        private void toolStripMenuItem_Read_All_Click(object sender, EventArgs e)
        {
            if (this.serialPort1.IsOpen == true)
            {
                is_read = 1;
            }
        }
        private void toolStripMenuItem_Write_All_Click(object sender, EventArgs e)
        {
            if (this.serialPort1.IsOpen == true)
            {
                is_write = 1;
            }
        }
        private void toolStripMenuItem_Page_Page_Click(object sender, EventArgs e)
        {
            if (this.form1PageNo == 0 )
            {
                this.form1PageNo = 1;
            }
            else
            {
                this.form1PageNo = 0;
            }

            for (int i = 0; i < this.labelYArray.Length; i++)
            {
                this.labelYArray[i].Text = ascii[((i / 16) % 16) + (this.form1PageNo * 2)] + this.ascii[i % 16] + "0";
            }
            for (int i = 0; i < this.textBoxArray.Length; i++)
            {
                this.textBoxArray[i].Text = this.EepromData[i + this.form1PageNo * 16 * 2 * 16];
            }
        }
        private void toolStripMenuItem_Option_Stop_Click(object sender, EventArgs e)
        {
            is_error_clear = 1;
        }

        /* --------------------タイマ割り込み-------------------- */
        private void timer_elapsed(object sender, System.Timers.ElapsedEventArgs e)
        {
            /* 1ms毎に実行する処理 */
            {
                if ( uart_trans_time_ms < UInt16.MaxValue )
                {
                    uart_trans_time_ms++;
                }
                
                state_machine();
                serialPort1_DataTrans();

            }
        }
        /* --------------------UART-------------------- */
        enum E_UART_FORMAT:byte
        {
            UART_FORMAT_CONTROL,
            UART_FORMAT_EEPROM_SIZE,
            UART_FORMAT_EEPROM_ADDRESS,
            UART_FORMAT_EEPROM_DATA,
            UART_FORMAT_MAX
        };

        public static class Define
        {
            /* 通信データ(数値)の長さ(BYTE単位) */
            /* ※必ずテーブルUART_FORMAT_LENGTH[]の合計値とすること */
            public const byte UART_DATA_LENGTH_CHAR = 35;

            /* 通信フレームデータ(ASCII)の長さ(BYTE単位) */
            public const byte UART_STX_LENGTH = 1;
            public const byte UART_CRC_LENGTH = 8;
            public const byte UART_ETX_LENGTH = 1;
            public const byte UART_DATA_LENGTH = (UART_STX_LENGTH + (UART_DATA_LENGTH_CHAR * 2));
            public const byte UART_FRAME_LENGTH = (UART_DATA_LENGTH + UART_CRC_LENGTH + UART_ETX_LENGTH);
            //public const int  UART_TRANS_INTERBAL_MS = (int)( (double)(UART_FRAME_LENGTH * 10 ) * 1000.0 /9600.0 ) + 10; /* 送信バイト x 1bytebit長 x ボーレート(ms換算) + 10ms */
            public const int UART_TRANS_INTERBAL_MS = (int)1000; /* 送信バイト x 1bytebit長 x ボーレート(ms換算) + 10ms */
            public const byte UART_STX_ASCII = 0x02;
            public const byte UART_ETX_ASCII = 0x03;
        }
        /* 通信データ(数値)の各要素ごとの長さ(BYTE単位) */
        /* 変更したら必ずUART_DATA_LENGTH_CHARを修正すること */
        private static  byte[] UART_FORMAT_LENGTH = new byte[ (byte)E_UART_FORMAT.UART_FORMAT_MAX ] 
        {
            1, /* UART_FORMAT_CONTROL        */
            1, /* UART_FORMAT_EEPROM_SIZE    */
            1, /* UART_FORMAT_EEPROM_ADDRESS */
           32, /* UART_FORMAT_EEPROM_DATA    */
        };

        private byte uart_format_get_idx( byte num)
        {
            byte ret = 0;
            if (num != 0)
            {
                byte i;
                for (i = 0; i < num; i++)
                {
                    ret += UART_FORMAT_LENGTH[i];
                }
            }
            return (ret);
        }

        private byte uart_format_get_length( byte num)
        {
            return (UART_FORMAT_LENGTH[num]);
        }

         static byte[] uart_recieve_frame = new byte[Define.UART_FRAME_LENGTH];
         static byte[] uart_recieve_char_data = new byte[Define.UART_DATA_LENGTH_CHAR];
         static byte[] uart_trans_frame = new byte[Define.UART_FRAME_LENGTH];
         static byte[] uart_trans_char_data = new byte[Define.UART_DATA_LENGTH_CHAR];

        private uint get_crc32( byte[] array ) /* 計算法が間違ってるっぽい。送受信で同じ計算法なら良いので取りあえずヨシ */
        {
            const uint CRC32 = 0x04C11DB7;

            uint crc = 0xFFFFFFFF;
            byte i;
            for (i = Define.UART_STX_LENGTH; i < Define.UART_DATA_LENGTH; i++)
            {
                byte k;
                for (k = 0; k < 8; k++)
                {
                    uint msb = (crc & 0x80000000);
                    /* crcを1bit左シフト */
                    {
                        crc = (crc << 1);
                        if (((array[i] << k) & 0x80) != 0)
                        {
                            crc |= (uint)1;
                        }
                    }
                    /* 溢れたbitが1ならXOR */
                    if (msb != 0)
                    {
                        crc ^= CRC32;
                    }
                }
            }
            return (crc ^ 0xFFFFFFFF);
        }
        private byte get_char_to_ascii(byte char_data)
        {
            byte ascii_data = (byte)'0';

            if (char_data <= 0x0F)
            {
                ascii_data = System.Text.Encoding.ASCII.GetBytes(this.ascii[char_data])[0];
            }

            return (ascii_data);
        }
        private byte get_ascii_to_char(byte ascii_data_h, byte ascii_data_l)
        {
            byte char_data = 0;
            byte char_data_h = 0;
            byte char_data_l = 0;
            byte i;

            for (i = 0; i < 16; i++)
            {
                if (ascii_data_h == System.Text.Encoding.ASCII.GetBytes(this.ascii[i])[0])
                {
                    char_data_h = i;
                }
                if (ascii_data_l == System.Text.Encoding.ASCII.GetBytes(this.ascii[i])[0])
                {
                    char_data_l = i;
                }
            }

            char_data = (byte)((char_data_h << 4) | char_data_l);

            return (char_data);
        }
        private void serialPort1_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            string read_line;
            byte[] data;

            if (this.serialPort1.IsOpen == true)
            {
                try
                {
                    this.serialPort1.NewLine = ((char)0x03).ToString();

                    read_line = this.serialPort1.ReadLine();
                    data = System.Text.Encoding.ASCII.GetBytes(read_line);

                    /* フレーム長チェック */
                    if (data.Length == (Define.UART_FRAME_LENGTH - Define.UART_ETX_LENGTH))
                    {
                        bool crc_flag = true;
                        uart_recieve_frame = data;
                        /* CRCチェック */
                        {
                            uint crc = get_crc32(uart_recieve_frame);
                            byte i;
                            for (i = 0; i < Define.UART_CRC_LENGTH; i++)
                            {
                                byte j = (byte)(4 * ((-1 - i) & 0x07)); /* なんかダサい。。。良い方法はないか？ */
                                uint mask = (0xF0000000 >> (4 * i));
                                byte a = uart_recieve_frame[Define.UART_DATA_LENGTH + i];
                                byte b = get_char_to_ascii((byte)((crc & mask) >> j));
                                if (uart_recieve_frame[Define.UART_DATA_LENGTH + i] != get_char_to_ascii((byte)((crc & mask) >> j)))
                                {
                                    crc_flag = false;
                                }
                            }

                        }

                        /* CRC一致なら採用する */
                        if (crc_flag != false)
                        {
                            byte i;
                            for (i = 0; i < Define.UART_DATA_LENGTH_CHAR; i++)
                            {

                                byte j = (byte)(Define.UART_STX_LENGTH + (2 * i));
                                uart_recieve_char_data[i] = get_ascii_to_char(uart_recieve_frame[j], uart_recieve_frame[j + 1]);
                            }
                        }
                    }
                }
                catch ( Exception ee ) { }

            }
        }
        private void serialPort1_DataTrans()
        {
            /* 送信フレーム生成 */
            {
                /* スタートコード付与 */
                uart_trans_frame[0] = Define.UART_STX_ASCII;

                /* 送信データ生成 */
                {
                    byte i;
                    for (i = Define.UART_STX_LENGTH; i < Define.UART_DATA_LENGTH; i++)
                    {
                        byte  j = (byte)((i - Define.UART_STX_LENGTH) / 2);
                        byte  l = (byte)((i - Define.UART_STX_LENGTH) & 0x01);
                        if (l == 0)
                        {
                            uart_trans_frame[i] = get_char_to_ascii((byte)((uart_trans_char_data[j] & 0xF0) >> 4));
                        }
                        else
                        {
                            uart_trans_frame[i] = get_char_to_ascii((byte)(uart_trans_char_data[j] & 0x0F));
                        }
                    }
                }

                /* CRC32付与 */
                {
                    uint crc = get_crc32(uart_trans_frame);
                    byte i;
                    for (i = 0; i < Define.UART_CRC_LENGTH; i++)
                    {
                        byte j = (byte)(4 * ((-1 - i) & 0x07)); /* なんかダサい。。。良い方法はないか？ */
                        uint mask = (0xF0000000 >> (4 * i));
                        uart_trans_frame[Define.UART_DATA_LENGTH + i] = get_char_to_ascii((byte)((crc & mask) >> j));
                    }
                }

                /* エンドコード付与 */
                uart_trans_frame[Define.UART_FRAME_LENGTH - 1] = Define.UART_ETX_ASCII;
            }

            if ( this.serialPort1.IsOpen == true )
            {
                string text = System.Text.Encoding.ASCII.GetString(uart_trans_frame);
                this.serialPort1.Write(text);

                uart_trans_time_ms = 0;
            }
        }

        /* --------------------状態遷移-------------------- */
        enum E_STATE:byte
        {
            STATE_WAIT,
            STATE_WRITE_ORDER,
            STATE_WRITE_FINISH,
            STATE_READ_ORDER,
            STATE_READ_FINISH,
            STATE_ERROR,
            STATE_MAX
        };
        static byte state = (byte)E_STATE.STATE_WAIT;
        private void state_machine()
        {
            if (uart_recieve_char_data[0] == (byte)E_UART_CONTROL.UART_CONTROL_ERROR )
            {
                state = (byte)E_STATE.STATE_ERROR;
            }
            else if ((uart_recieve_char_data[0] != (byte)E_UART_CONTROL.UART_CONTROL_WAIT ) &&( is_error_clear == 1) )
            {
                state = (byte)E_STATE.STATE_ERROR;
            }
            else
            { 
                /* 次状態決定 & 退場処理 */
                switch (state)
                {
                    case (byte)E_STATE.STATE_WAIT:
                        if (is_write != 0)
                        {
                            byte write_ok = 1;
                            Int64 i;
                            for ( i = 0; i < EepromData.Length; i++ )
                            {
                                try
                                {
                                    byte tmp = Convert.ToByte(EepromData[i], 16);
                                }
                                catch
                                {
                                    write_ok = 0;
                                    break;
                                }
                            }

                            if ( write_ok != 0 )
                            {
                                write_idx = (byte)0;
                                state = (byte)E_STATE.STATE_WRITE_ORDER;
                            }

                            is_write = 0;

                        }
                        else if (is_read != 0)
                        {
                            read_idx = (byte)0;
                            state = (byte)E_STATE.STATE_READ_ORDER;

                            is_read = 0;
                        }
                        else
                        {
                            state = state;
                        }

                        is_error_clear = 0;

                        break;
                    case (byte)E_STATE.STATE_WRITE_ORDER:
                        if (uart_recieve_char_data[0] == (byte)E_UART_CONTROL.UART_CONTROL_WRITE_FINISH)
                        {
                            state = (byte)E_STATE.STATE_WRITE_FINISH;
                        }
                        else
                        {
                            state = state;
                        }
                        break;
                    case (byte)E_STATE.STATE_WRITE_FINISH:
                        if (uart_recieve_char_data[0] == (byte)E_UART_CONTROL.UART_CONTROL_WAIT)
                        {
                            write_idx++;
                            state = (byte)E_STATE.STATE_WRITE_ORDER;
                        }
                        else
                        {
                            state = state;
                        }
                        if (write_idx >= EEP_PARAM[this.eeprom_idx].idx_max)
                        {
                            state = (byte)E_STATE.STATE_WAIT;
                        }
                        break;
                    case (byte)E_STATE.STATE_READ_ORDER:
                        if (uart_recieve_char_data[0] == (byte)E_UART_CONTROL.UART_CONTROL_READ_FINISH)
                        {
                            state = (byte)E_STATE.STATE_READ_FINISH;
                        }
                        else
                        {
                            state = state;
                        }
                        break;
                    case (byte)E_STATE.STATE_READ_FINISH:
                        if (uart_recieve_char_data[0] == (byte)E_UART_CONTROL.UART_CONTROL_WAIT)
                        {
                            {
                                byte i;
                                for (i = 0; i < UART_FORMAT_LENGTH[(int)E_UART_FORMAT.UART_FORMAT_EEPROM_DATA]; i++)
                                {
                                    EepromData[read_idx * UART_FORMAT_LENGTH[(int)E_UART_FORMAT.UART_FORMAT_EEPROM_DATA] + i] = uart_recieve_char_data[3 + i].ToString("X2");

                                }
                                Invoke((MethodInvoker)(() =>
                                {
                                    int j;
                                    for ( j = 0; j < this.textBoxArray.Length; j++)
                                    {
                                        this.textBoxArray[j].Text = this.EepromData[j + this.form1PageNo * 16 * 2 * 16];
                                    }
                                }));

                            }
                            read_idx++;
                            state = (byte)E_STATE.STATE_READ_ORDER;
                        }
                        else
                        {
                            state = state;
                        }

                        if (read_idx >= EEP_PARAM[this.eeprom_idx].idx_max)
                        {
                            state = (byte)E_STATE.STATE_WAIT;
                        }
                        break;
                    case (byte)E_STATE.STATE_ERROR:
                        if (uart_recieve_char_data[0] == (byte)E_UART_CONTROL.UART_CONTROL_WAIT)
                        {
                            state = (byte)E_STATE.STATE_WAIT;
                        }
                        break;
                    default:

                        break;
                }
            }
            /* 定常処理 */
                switch (state)
                {
                    case (byte)E_STATE.STATE_WAIT :
                        uart_trans_char_data[0] = (byte)E_UART_CONTROL.UART_CONTROL_WAIT;
                        break;

                    case (byte)E_STATE.STATE_WRITE_ORDER :
                        {
                            byte i;
                            for ( i = 0; i < UART_FORMAT_LENGTH[(int)E_UART_FORMAT.UART_FORMAT_EEPROM_DATA]; i++ )
                            {
                                uart_trans_char_data[3 + i] = Convert.ToByte(EepromData[write_idx * UART_FORMAT_LENGTH[(int)E_UART_FORMAT.UART_FORMAT_EEPROM_DATA] + i], 16);
                            }
                        }
                        uart_trans_char_data[2] = write_idx;
                        uart_trans_char_data[1] = EEP_PARAM[this.eeprom_idx].size_id;
                        uart_trans_char_data[0] = (byte)E_UART_CONTROL.UART_CONTROL_WRITE_ORDER;
                        break;
                    case (byte)E_STATE.STATE_WRITE_FINISH :
                        uart_trans_char_data[0] = (byte)E_UART_CONTROL.UART_CONTROL_WAIT_ORDER;
                        break;
                    case (byte)E_STATE.STATE_READ_ORDER :
                        uart_trans_char_data[2] = read_idx;
                        uart_trans_char_data[1] = EEP_PARAM[this.eeprom_idx].size_id;
                        uart_trans_char_data[0] = (byte)E_UART_CONTROL.UART_CONTROL_READ_ORDER;
                        break;
                    case (byte)E_STATE.STATE_READ_FINISH :
                        uart_trans_char_data[0] = (byte)E_UART_CONTROL.UART_CONTROL_WAIT_ORDER;
                        break;
                    case (byte)E_STATE.STATE_ERROR :
                        if ( is_error_clear !=  0)
                        {
                            uart_trans_char_data[0] = (byte)E_UART_CONTROL.UART_CONTROL_WAIT_ORDER;
                        }
                    break;
                    default :

                        break;
                }
        }
    

    
    }
}
