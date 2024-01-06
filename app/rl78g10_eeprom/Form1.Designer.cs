namespace rl78g10_eeprom
{
    partial class Form1
    {
        /// <summary>
        /// 必要なデザイナー変数です。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        private System.Windows.Forms.OpenFileDialog openFileDialog1;
        private System.IO.Ports.SerialPort serialPort1;
//        private System.Windows.Forms.Timer timer1_1ms;
        private System.Timers.Timer timer;

        private System.Windows.Forms.Label[] labelYArray;
        private System.Windows.Forms.Label[] labelXArray;
        private System.Windows.Forms.TextBox[] textBoxArray;

        private System.Windows.Forms.MenuStrip menuStrip1;
            private System.Windows.Forms.ToolStripMenuItem toolStripMenuItem_Connect;
                private System.Windows.Forms.ToolStripMenuItem toolStripMenuItem_Connect_Com;
                    private System.Windows.Forms.ToolStripComboBox toolStripConmboBox_Connect_Com;
                private System.Windows.Forms.ToolStripMenuItem toolStripMenuItem_Connect_EepSize;
                    private System.Windows.Forms.ToolStripComboBox toolStripConmboBox_Connect_EepSize;
                private System.Windows.Forms.ToolStripMenuItem toolStripMenuItem_Connect_Connect;
                private System.Windows.Forms.ToolStripMenuItem toolStripMenuItem_Connect_Disconnect;
            private System.Windows.Forms.ToolStripMenuItem toolStripMenuItem_File;
                private System.Windows.Forms.ToolStripMenuItem toolStripMenuItem_File_CsvInput;
                private System.Windows.Forms.ToolStripMenuItem toolStripMenuItem_File_CsvOutput;
            private System.Windows.Forms.ToolStripMenuItem toolStripMenuItem_Read;
                private System.Windows.Forms.ToolStripMenuItem toolStripMenuItem_Read_All;
            private System.Windows.Forms.ToolStripMenuItem toolStripMenuItem_Write;
                private System.Windows.Forms.ToolStripMenuItem toolStripMenuItem_Write_All;
            private System.Windows.Forms.ToolStripMenuItem toolStripMenuItem_Verify;
                private System.Windows.Forms.ToolStripMenuItem toolStripMenuItem_Verify_Verify;
            private System.Windows.Forms.ToolStripMenuItem toolStripMenuItem_Page;
                private System.Windows.Forms.ToolStripMenuItem toolStripMenuItem_Page_Page;
            private System.Windows.Forms.ToolStripMenuItem toolStripMenuItem_Option;
                private System.Windows.Forms.ToolStripMenuItem toolStripMenuItem_Option_Stop;

        /// <summary>
        /// 使用中のリソースをすべてクリーンアップします。
        /// </summary>
        /// <param name="disposing">マネージド リソースを破棄する場合は true を指定し、その他の場合は false を指定します。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(800, 450);
            this.Text = "Form1";

            this.openFileDialog1 = new System.Windows.Forms.OpenFileDialog();
            this.serialPort1 = new System.IO.Ports.SerialPort(this.components);
//            this.timer1_1ms = new System.Windows.Forms.Timer(this.components);
            this.timer = new System.Timers.Timer();

            this.labelXArray = new System.Windows.Forms.Label[16];
            this.labelYArray = new System.Windows.Forms.Label[16*2];
            this.textBoxArray = new System.Windows.Forms.TextBox[16*2*16];

            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
                this.toolStripMenuItem_Connect = new System.Windows.Forms.ToolStripMenuItem();
                    this.toolStripMenuItem_Connect_Com = new System.Windows.Forms.ToolStripMenuItem();
                        this.toolStripConmboBox_Connect_Com = new System.Windows.Forms.ToolStripComboBox();
                    this.toolStripMenuItem_Connect_EepSize = new System.Windows.Forms.ToolStripMenuItem();
                        this.toolStripConmboBox_Connect_EepSize = new System.Windows.Forms.ToolStripComboBox();
                    this.toolStripMenuItem_Connect_Connect = new System.Windows.Forms.ToolStripMenuItem();
                    this.toolStripMenuItem_Connect_Disconnect = new System.Windows.Forms.ToolStripMenuItem();
                this.toolStripMenuItem_File = new System.Windows.Forms.ToolStripMenuItem();
                    this.toolStripMenuItem_File_CsvInput = new System.Windows.Forms.ToolStripMenuItem();
                    this.toolStripMenuItem_File_CsvOutput = new System.Windows.Forms.ToolStripMenuItem();
                this.toolStripMenuItem_Read = new System.Windows.Forms.ToolStripMenuItem();
                    this.toolStripMenuItem_Read_All = new System.Windows.Forms.ToolStripMenuItem();
                this.toolStripMenuItem_Write = new System.Windows.Forms.ToolStripMenuItem();
                    this.toolStripMenuItem_Write_All = new System.Windows.Forms.ToolStripMenuItem();
                this.toolStripMenuItem_Verify = new System.Windows.Forms.ToolStripMenuItem();
                    this.toolStripMenuItem_Verify_Verify = new System.Windows.Forms.ToolStripMenuItem();
                this.toolStripMenuItem_Page = new System.Windows.Forms.ToolStripMenuItem();
                    this.toolStripMenuItem_Page_Page = new System.Windows.Forms.ToolStripMenuItem();
                this.toolStripMenuItem_Option = new System.Windows.Forms.ToolStripMenuItem();
                    this.toolStripMenuItem_Option_Stop = new System.Windows.Forms.ToolStripMenuItem();

            this.menuStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // openFileDialog1
            // 
            this.openFileDialog1.Filter = "CSV File|*.csv";
            // 
            // serialPort1
            // 
            this.serialPort1.ReadBufferSize = 1024;
            this.serialPort1.DataReceived += new System.IO.Ports.SerialDataReceivedEventHandler(this.serialPort1_DataReceived);
            // 
            // timer1_1ms
            // 
            //            this.timer1_1ms.Enabled = true;
            //            this.timer1_1ms.Interval = 1;
            //            this.timer1_1ms.Tick += new System.EventHandler(this.timer1_1ms_Tick);
            // timer
            this.timer.Interval = 90;
            this.timer.Elapsed += new System.Timers.ElapsedEventHandler(this.timer_elapsed);
            // 
            // labelXArray
            // 
            for (int i = 0; i < this.labelXArray.Length; i++)
            {
                this.labelXArray[i] = new System.Windows.Forms.Label();

                this.labelXArray[i].AutoSize = false;
                this.labelXArray[i].BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
                this.labelXArray[i].Font = new System.Drawing.Font("ＭＳ ゴシック", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
                this.labelXArray[i].Location = new System.Drawing.Point(40 + i * 20, 40);
                this.labelXArray[i].Name = "label" + (i.ToString());
                this.labelXArray[i].Size = new System.Drawing.Size(20, 14);
                this.labelXArray[i].TabIndex = 2;
                this.labelXArray[i].Text = " " + this.ascii[i];
            }
            // 
            // labelYArray
            // 
            for (int i = 0; i < this.labelYArray.Length; i++)
            {
                this.labelYArray[i] = new System.Windows.Forms.Label();

                this.labelYArray[i].AutoSize = false;
                this.labelYArray[i].BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
                this.labelYArray[i].Font = new System.Drawing.Font("ＭＳ ゴシック", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
                this.labelYArray[i].Location = new System.Drawing.Point(10, 60+i*16);
                this.labelYArray[i].Name = "label" + (i.ToString());
                this.labelYArray[i].Size = new System.Drawing.Size(27, 14);
                this.labelYArray[i].TabIndex = 34;
                this.labelYArray[i].Text = this.ascii[((i/16)%16)+(this.form1PageNo *2)]+ this.ascii[i%16]+"0";
            }
            // 
            // textBoxArray
            // 
            for ( int i = 0; i < this.textBoxArray.Length; i++ )
            {
                this.textBoxArray[i] = new System.Windows.Forms.TextBox();

                int x = i % 16;
                int y = i / 16;

                this.textBoxArray[i].AutoSize = false;
                this.textBoxArray[i].TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
                this.textBoxArray[i].Font = new System.Drawing.Font("ＭＳ ゴシック", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
                this.textBoxArray[i].Location = new System.Drawing.Point(40+x*20, 60+y*16);
                this.textBoxArray[i].Name = "textbox" + (i.ToString());
                this.textBoxArray[i].Size = new System.Drawing.Size(18, 14);
                this.textBoxArray[i].TabIndex = 64;
                this.textBoxArray[i].Text = this.form1CsvData[i+ this.form1PageNo *16*2*16];
            }
            // 
            // menuStrip1
            // 
            this.menuStrip1.BackColor = System.Drawing.SystemColors.Control;
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[]
            {
                this.toolStripMenuItem_Connect,
                this.toolStripMenuItem_File,
                this.toolStripMenuItem_Read,
                this.toolStripMenuItem_Write,
                this.toolStripMenuItem_Verify,
                this.toolStripMenuItem_Page,
                this.toolStripMenuItem_Option
            });
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(800, 24);
            this.menuStrip1.TabIndex = 5;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // toolStripMenuItem_Connect
            // 
            this.toolStripMenuItem_Connect.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripMenuItem_Connect_Com,
            this.toolStripMenuItem_Connect_EepSize,
            this.toolStripMenuItem_Connect_Connect,
            this.toolStripMenuItem_Connect_Disconnect});
            this.toolStripMenuItem_Connect.Name = "toolStripMenuItem_Connect";
            this.toolStripMenuItem_Connect.Text = "接続";

            // 
            // toolStripMenuItem_Connect_Com
            // 
            this.toolStripMenuItem_Connect_Com.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripConmboBox_Connect_Com});
            this.toolStripMenuItem_Connect_Com.Name = "toolStripMenuItem_Connect_Com";
            this.toolStripMenuItem_Connect_Com.Text = "COMポート";
            this.toolStripMenuItem_Connect_Com.Click += new System.EventHandler(this.toolStripMenuItem_Connect_Com_Click);
            // 
            // toolStripConmboBox_Connect_Com
            // 
            this.toolStripConmboBox_Connect_Com.Name = "toolStripConmboBox_Connect_Com";
            this.toolStripConmboBox_Connect_Com.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            //            this.toolStripConmboBox_Connect_Com.Click += new System.EventHandler(this.toolStripComboBox2_Click)
            // 
            // toolStripMenuItem_Connect_EepSize
            // 
            this.toolStripMenuItem_Connect_EepSize.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripConmboBox_Connect_EepSize});
            this.toolStripMenuItem_Connect_EepSize.Name = "toolStripMenuItem_Connect_EepSize";
            this.toolStripMenuItem_Connect_EepSize.Text = "EEPROMサイズ";
            // 
            // toolStripConmboBox_Connect_EepSize
            // 
            this.toolStripConmboBox_Connect_EepSize.Name = "toolStripConmboBox_Connect_EepSize";
            this.toolStripConmboBox_Connect_EepSize.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
//            this.toolStripConmboBox_Connect_EepSize.Click += new System.EventHandler(this.toolStripComboBox2_Click);
            // 
            // toolStripMenuItem_Connect_Connect
            // 
            this.toolStripMenuItem_Connect_Connect.Name = "toolStripMenuItem_Connect_Connect";
            this.toolStripMenuItem_Connect_Connect.Text = "接続";
            this.toolStripMenuItem_Connect_Connect.Click += new System.EventHandler(this.toolStripMenuItem_Connect_Connect_Click);
            // 
            // toolStripMenuItem_Connect_Disconnect
            // 
            this.toolStripMenuItem_Connect_Disconnect.Name = "toolStripMenuItem_Connect_Disconnect";
            this.toolStripMenuItem_Connect_Disconnect.Text = "切断";
            this.toolStripMenuItem_Connect_Disconnect.Click += new System.EventHandler(this.toolStripMenuItem_Connect_Disconnect_Click);
            // 
            // 
            // toolStripMenuItem_File
            // 
            this.toolStripMenuItem_File.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripMenuItem_File_CsvInput,
            this.toolStripMenuItem_File_CsvOutput});
            this.toolStripMenuItem_File.Name = "toolStripMenuItem_File";
            this.toolStripMenuItem_File.Text = "ファイル";
            // 
            // toolStripMenuItem_File_CsvInput
            // 
            this.toolStripMenuItem_File_CsvInput.Name = "toolStripMenuItem_File_CsvInput";
            this.toolStripMenuItem_File_CsvInput.Text = "CSV読み込み...";
            this.toolStripMenuItem_File_CsvInput.Click += new System.EventHandler(this.toolStripMenuItem_File_CsvInput_Click);
            // 
            // toolStripMenuItem_File_CsvOutput
            // 
            this.toolStripMenuItem_File_CsvOutput.Name = "toolStripMenuItem_File_CsvOutput";
            this.toolStripMenuItem_File_CsvOutput.Text = "CSV出力...";
//            this.toolStripMenuItem_File_CsvOutput.Click += new System.EventHandler(this.toolStripMenuItem3_Click);
            // 
            // toolStripMenuItem_Read
            // 
            this.toolStripMenuItem_Read.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripMenuItem_Read_All});
            this.toolStripMenuItem_Read.Name = "toolStripMenuItem_Read";
            this.toolStripMenuItem_Read.Text = "読込";
            // 
            // toolStripMenuItem_Read_All
            // 
            this.toolStripMenuItem_Read_All.Name = "toolStripMenuItem_Read_All";
            this.toolStripMenuItem_Read_All.Text = "全読み込み";
            this.toolStripMenuItem_Read_All.Click += new System.EventHandler(this.toolStripMenuItem_Read_All_Click);
            // 
            // toolStripMenuItem_Write
            // 
            this.toolStripMenuItem_Write.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripMenuItem_Write_All});
            this.toolStripMenuItem_Write.Name = "toolStripMenuItem_Write";
            this.toolStripMenuItem_Write.Text = "書込";
            // 
            // toolStripMenuItem_Write_All
            // 
            this.toolStripMenuItem_Write_All.Name = "toolStripMenuItem_Write_All";
            this.toolStripMenuItem_Write_All.Text = "全書き込み";
            this.toolStripMenuItem_Write_All.Click += new System.EventHandler(this.toolStripMenuItem_Write_All_Click);
            // 
            // toolStripMenuItem_Verify
            // 
            this.toolStripMenuItem_Verify.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripMenuItem_Verify_Verify});
            this.toolStripMenuItem_Verify.Name = "toolStripMenuItem_Verify";
            this.toolStripMenuItem_Verify.Text = "ベリファイ";
            // 
            // toolStripMenuItem_Verify_Verify
            // 
            this.toolStripMenuItem_Verify_Verify.Name = "toolStripMenuItem_Verify_Verify";
            this.toolStripMenuItem_Verify_Verify.Text = "ベリファイ実施";
//            this.toolStripMenuItem_Verify_Verify.Click += new System.EventHandler(this.toolStripMenuItem3_Click);
            // 
            // toolStripMenuItem_Page
            // 
            this.toolStripMenuItem_Page.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripMenuItem_Page_Page});
            this.toolStripMenuItem_Page.Name = "toolStripMenuItem_Page";
            this.toolStripMenuItem_Page.Text = "ページ";
            // 
            // toolStripMenuItem_Page_Page
            // 
            this.toolStripMenuItem_Page_Page.Name = "toolStripMenuItem_Page_Page";
            this.toolStripMenuItem_Page_Page.Text = "ページ切替";
            this.toolStripMenuItem_Page_Page.Click += new System.EventHandler(this.toolStripMenuItem_Page_Page_Click);
           // 
            // toolStripMenuItem_Option
            // 
            this.toolStripMenuItem_Option.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripMenuItem_Option_Stop});
            this.toolStripMenuItem_Option.Name = "toolStripMenuItem_Option";
            this.toolStripMenuItem_Option.Text = "オプション";
            // 
            // toolStripMenuItem_Option_Stop
            // 
            this.toolStripMenuItem_Option_Stop.Name = "toolStripMenuItem_Option_Stop";
            this.toolStripMenuItem_Option_Stop.Text = "強制終了";
            this.toolStripMenuItem_Option_Stop.Click += new System.EventHandler(this.toolStripMenuItem_Option_Stop_Click);

            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(400, 600);
            this.Controls.Add(this.menuStrip1);
            this.Controls.AddRange(this.labelXArray);
            this.Controls.AddRange(this.labelYArray);
            this.Controls.AddRange(this.textBoxArray);
            this.MainMenuStrip = this.menuStrip1;
            this.Name = "Form1";
            this.Text = "Form1";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();
        }

    }
}

