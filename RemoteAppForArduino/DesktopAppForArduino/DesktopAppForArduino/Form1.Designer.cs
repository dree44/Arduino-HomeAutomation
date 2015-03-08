namespace DesktopAppForArduino
{
    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.sendBox = new System.Windows.Forms.TextBox();
            this.panel1 = new System.Windows.Forms.Panel();
            this.localPortText = new System.Windows.Forms.Label();
            this.portText = new System.Windows.Forms.Label();
            this.localIPBox = new System.Windows.Forms.TextBox();
            this.ipBox = new System.Windows.Forms.TextBox();
            this.ipText = new System.Windows.Forms.Label();
            this.localIPText = new System.Windows.Forms.Label();
            this.remotePortBox = new System.Windows.Forms.TextBox();
            this.localPortBox = new System.Windows.Forms.TextBox();
            this.sendPanel = new System.Windows.Forms.Panel();
            this.dlBasicButton = new System.Windows.Forms.Button();
            this.dlIOButton = new System.Windows.Forms.Button();
            this.inputSaveButton = new System.Windows.Forms.Button();
            this.inputLoadButton = new System.Windows.Forms.Button();
            this.inputClearButton = new System.Windows.Forms.Button();
            this.statusButton = new System.Windows.Forms.Button();
            this.sendButton = new System.Windows.Forms.Button();
            this.helpButton = new System.Windows.Forms.Button();
            this.button3 = new System.Windows.Forms.Button();
            this.button2 = new System.Windows.Forms.Button();
            this.listButton = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.panel2 = new System.Windows.Forms.Panel();
            this.clearAllButton = new System.Windows.Forms.Button();
            this.outputSaveButton = new System.Windows.Forms.Button();
            this.packetNumBox = new System.Windows.Forms.TextBox();
            this.minusButton = new System.Windows.Forms.Button();
            this.plusButton = new System.Windows.Forms.Button();
            this.outputClearButton = new System.Windows.Forms.Button();
            this.receiveBox = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.inputOpenFileDialog = new System.Windows.Forms.OpenFileDialog();
            this.inputSaveFileDialog = new System.Windows.Forms.SaveFileDialog();
            this.outputSaveFileDialog = new System.Windows.Forms.SaveFileDialog();
            this.timer = new System.Windows.Forms.Timer(this.components);
            this.panel1.SuspendLayout();
            this.sendPanel.SuspendLayout();
            this.panel2.SuspendLayout();
            this.SuspendLayout();
            // 
            // sendBox
            // 
            this.sendBox.AcceptsReturn = true;
            this.sendBox.AcceptsTab = true;
            this.sendBox.Font = new System.Drawing.Font("Lucida Console", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(238)));
            this.sendBox.Location = new System.Drawing.Point(3, 48);
            this.sendBox.Multiline = true;
            this.sendBox.Name = "sendBox";
            this.sendBox.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.sendBox.Size = new System.Drawing.Size(522, 457);
            this.sendBox.TabIndex = 0;
            this.sendBox.WordWrap = false;
            // 
            // panel1
            // 
            this.panel1.AutoSize = true;
            this.panel1.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.panel1.Controls.Add(this.localPortText);
            this.panel1.Controls.Add(this.portText);
            this.panel1.Controls.Add(this.localIPBox);
            this.panel1.Controls.Add(this.ipBox);
            this.panel1.Controls.Add(this.ipText);
            this.panel1.Controls.Add(this.localIPText);
            this.panel1.Controls.Add(this.remotePortBox);
            this.panel1.Controls.Add(this.localPortBox);
            this.panel1.Location = new System.Drawing.Point(16, 13);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(1038, 35);
            this.panel1.TabIndex = 1;
            // 
            // localPortText
            // 
            this.localPortText.AutoSize = true;
            this.localPortText.Location = new System.Drawing.Point(805, 9);
            this.localPortText.Name = "localPortText";
            this.localPortText.Size = new System.Drawing.Size(81, 13);
            this.localPortText.TabIndex = 6;
            this.localPortText.Text = "Local UDP Port";
            // 
            // portText
            // 
            this.portText.AutoSize = true;
            this.portText.Location = new System.Drawing.Point(235, 9);
            this.portText.Name = "portText";
            this.portText.Size = new System.Drawing.Size(92, 13);
            this.portText.TabIndex = 2;
            this.portText.Text = "Remote UDP Port";
            // 
            // localIPBox
            // 
            this.localIPBox.Enabled = false;
            this.localIPBox.Location = new System.Drawing.Point(674, 6);
            this.localIPBox.Name = "localIPBox";
            this.localIPBox.Size = new System.Drawing.Size(125, 20);
            this.localIPBox.TabIndex = 5;
            // 
            // ipBox
            // 
            this.ipBox.Location = new System.Drawing.Point(104, 6);
            this.ipBox.Name = "ipBox";
            this.ipBox.Size = new System.Drawing.Size(125, 20);
            this.ipBox.TabIndex = 1;
            this.ipBox.Text = "192.168.2.1";
            this.ipBox.Leave += new System.EventHandler(this.ipBox_Leave);
            // 
            // ipText
            // 
            this.ipText.AutoSize = true;
            this.ipText.Location = new System.Drawing.Point(3, 9);
            this.ipText.Name = "ipText";
            this.ipText.Size = new System.Drawing.Size(98, 13);
            this.ipText.TabIndex = 3;
            this.ipText.Text = "Remote IP Address";
            // 
            // localIPText
            // 
            this.localIPText.AutoSize = true;
            this.localIPText.Location = new System.Drawing.Point(573, 9);
            this.localIPText.Name = "localIPText";
            this.localIPText.Size = new System.Drawing.Size(87, 13);
            this.localIPText.TabIndex = 7;
            this.localIPText.Text = "Local IP Address";
            // 
            // remotePortBox
            // 
            this.remotePortBox.Location = new System.Drawing.Point(333, 6);
            this.remotePortBox.Name = "remotePortBox";
            this.remotePortBox.Size = new System.Drawing.Size(125, 20);
            this.remotePortBox.TabIndex = 0;
            this.remotePortBox.Text = "65000";
            this.remotePortBox.Leave += new System.EventHandler(this.portBox_Leave);
            // 
            // localPortBox
            // 
            this.localPortBox.Location = new System.Drawing.Point(903, 6);
            this.localPortBox.Name = "localPortBox";
            this.localPortBox.Size = new System.Drawing.Size(125, 20);
            this.localPortBox.TabIndex = 4;
            this.localPortBox.Text = "65000";
            this.localPortBox.Leave += new System.EventHandler(this.localPortBox_Leave);
            // 
            // sendPanel
            // 
            this.sendPanel.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.sendPanel.Controls.Add(this.dlBasicButton);
            this.sendPanel.Controls.Add(this.dlIOButton);
            this.sendPanel.Controls.Add(this.inputSaveButton);
            this.sendPanel.Controls.Add(this.inputLoadButton);
            this.sendPanel.Controls.Add(this.inputClearButton);
            this.sendPanel.Controls.Add(this.statusButton);
            this.sendPanel.Controls.Add(this.sendButton);
            this.sendPanel.Controls.Add(this.helpButton);
            this.sendPanel.Controls.Add(this.button3);
            this.sendPanel.Controls.Add(this.button2);
            this.sendPanel.Controls.Add(this.listButton);
            this.sendPanel.Controls.Add(this.label1);
            this.sendPanel.Controls.Add(this.sendBox);
            this.sendPanel.Location = new System.Drawing.Point(17, 49);
            this.sendPanel.Name = "sendPanel";
            this.sendPanel.Size = new System.Drawing.Size(530, 727);
            this.sendPanel.TabIndex = 2;
            // 
            // dlBasicButton
            // 
            this.dlBasicButton.Location = new System.Drawing.Point(104, 511);
            this.dlBasicButton.Name = "dlBasicButton";
            this.dlBasicButton.Size = new System.Drawing.Size(95, 48);
            this.dlBasicButton.TabIndex = 13;
            this.dlBasicButton.Text = "Download \r\nbasic\r\nBASIC.JSN";
            this.dlBasicButton.UseVisualStyleBackColor = true;
            this.dlBasicButton.Click += new System.EventHandler(this.dlBasicButton_Click);
            // 
            // dlIOButton
            // 
            this.dlIOButton.Location = new System.Drawing.Point(3, 511);
            this.dlIOButton.Name = "dlIOButton";
            this.dlIOButton.Size = new System.Drawing.Size(95, 48);
            this.dlIOButton.TabIndex = 12;
            this.dlIOButton.Text = "Download \r\nI/O set SETTING.JSN";
            this.dlIOButton.UseVisualStyleBackColor = true;
            this.dlIOButton.Click += new System.EventHandler(this.dlIOButton_Click);
            // 
            // inputSaveButton
            // 
            this.inputSaveButton.Location = new System.Drawing.Point(84, 26);
            this.inputSaveButton.Name = "inputSaveButton";
            this.inputSaveButton.Size = new System.Drawing.Size(75, 23);
            this.inputSaveButton.TabIndex = 11;
            this.inputSaveButton.Text = "Save...";
            this.inputSaveButton.UseVisualStyleBackColor = true;
            this.inputSaveButton.Click += new System.EventHandler(this.inputSaveButton_Click);
            // 
            // inputLoadButton
            // 
            this.inputLoadButton.Location = new System.Drawing.Point(3, 26);
            this.inputLoadButton.Name = "inputLoadButton";
            this.inputLoadButton.Size = new System.Drawing.Size(75, 23);
            this.inputLoadButton.TabIndex = 10;
            this.inputLoadButton.Text = "Load...";
            this.inputLoadButton.UseVisualStyleBackColor = true;
            this.inputLoadButton.Click += new System.EventHandler(this.inputLoadButton_Click);
            // 
            // inputClearButton
            // 
            this.inputClearButton.Location = new System.Drawing.Point(450, 26);
            this.inputClearButton.Name = "inputClearButton";
            this.inputClearButton.Size = new System.Drawing.Size(75, 23);
            this.inputClearButton.TabIndex = 9;
            this.inputClearButton.Text = "clear";
            this.inputClearButton.UseVisualStyleBackColor = true;
            this.inputClearButton.Click += new System.EventHandler(this.inputClearButton_Click);
            // 
            // statusButton
            // 
            this.statusButton.Location = new System.Drawing.Point(286, 511);
            this.statusButton.Name = "statusButton";
            this.statusButton.Size = new System.Drawing.Size(75, 23);
            this.statusButton.TabIndex = 8;
            this.statusButton.Text = "status";
            this.statusButton.UseVisualStyleBackColor = true;
            this.statusButton.Click += new System.EventHandler(this.statusButton_Click);
            // 
            // sendButton
            // 
            this.sendButton.Location = new System.Drawing.Point(447, 511);
            this.sendButton.Name = "sendButton";
            this.sendButton.Size = new System.Drawing.Size(75, 48);
            this.sendButton.TabIndex = 7;
            this.sendButton.Text = "Send";
            this.sendButton.UseVisualStyleBackColor = true;
            this.sendButton.Click += new System.EventHandler(this.sendButton_Click);
            // 
            // helpButton
            // 
            this.helpButton.Location = new System.Drawing.Point(205, 536);
            this.helpButton.Name = "helpButton";
            this.helpButton.Size = new System.Drawing.Size(75, 23);
            this.helpButton.TabIndex = 6;
            this.helpButton.Text = "help";
            this.helpButton.UseVisualStyleBackColor = true;
            this.helpButton.Click += new System.EventHandler(this.helpButton_Click);
            // 
            // button3
            // 
            this.button3.Location = new System.Drawing.Point(104, 565);
            this.button3.Name = "button3";
            this.button3.Size = new System.Drawing.Size(95, 48);
            this.button3.TabIndex = 5;
            this.button3.Text = "Upload\r\nbasic\r\nBASIC.JSN";
            this.button3.UseVisualStyleBackColor = true;
            // 
            // button2
            // 
            this.button2.Location = new System.Drawing.Point(3, 565);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(95, 48);
            this.button2.TabIndex = 4;
            this.button2.Text = "Upload\r\nI/O set\r\nSETTING.JSN";
            this.button2.UseVisualStyleBackColor = true;
            // 
            // listButton
            // 
            this.listButton.Location = new System.Drawing.Point(205, 511);
            this.listButton.Name = "listButton";
            this.listButton.Size = new System.Drawing.Size(75, 23);
            this.listButton.TabIndex = 3;
            this.listButton.Text = "list IO";
            this.listButton.UseVisualStyleBackColor = true;
            this.listButton.Click += new System.EventHandler(this.listButton_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(224, 4);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(37, 13);
            this.label1.TabIndex = 1;
            this.label1.Text = "SEND";
            // 
            // panel2
            // 
            this.panel2.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.panel2.Controls.Add(this.clearAllButton);
            this.panel2.Controls.Add(this.outputSaveButton);
            this.panel2.Controls.Add(this.packetNumBox);
            this.panel2.Controls.Add(this.minusButton);
            this.panel2.Controls.Add(this.plusButton);
            this.panel2.Controls.Add(this.outputClearButton);
            this.panel2.Controls.Add(this.receiveBox);
            this.panel2.Controls.Add(this.label2);
            this.panel2.Location = new System.Drawing.Point(552, 49);
            this.panel2.Name = "panel2";
            this.panel2.Size = new System.Drawing.Size(502, 727);
            this.panel2.TabIndex = 3;
            // 
            // clearAllButton
            // 
            this.clearAllButton.Location = new System.Drawing.Point(217, 510);
            this.clearAllButton.Name = "clearAllButton";
            this.clearAllButton.Size = new System.Drawing.Size(75, 23);
            this.clearAllButton.TabIndex = 14;
            this.clearAllButton.Text = "clear ALL";
            this.clearAllButton.UseVisualStyleBackColor = true;
            this.clearAllButton.Click += new System.EventHandler(this.clearAllButton_Click);
            // 
            // outputSaveButton
            // 
            this.outputSaveButton.Location = new System.Drawing.Point(3, 27);
            this.outputSaveButton.Name = "outputSaveButton";
            this.outputSaveButton.Size = new System.Drawing.Size(75, 23);
            this.outputSaveButton.TabIndex = 12;
            this.outputSaveButton.Text = "Save...";
            this.outputSaveButton.UseVisualStyleBackColor = true;
            this.outputSaveButton.Click += new System.EventHandler(this.outputSaveButton_Click);
            // 
            // packetNumBox
            // 
            this.packetNumBox.Location = new System.Drawing.Point(395, 513);
            this.packetNumBox.Name = "packetNumBox";
            this.packetNumBox.ReadOnly = true;
            this.packetNumBox.Size = new System.Drawing.Size(77, 20);
            this.packetNumBox.TabIndex = 13;
            // 
            // minusButton
            // 
            this.minusButton.Location = new System.Drawing.Point(376, 511);
            this.minusButton.Name = "minusButton";
            this.minusButton.Size = new System.Drawing.Size(20, 23);
            this.minusButton.TabIndex = 12;
            this.minusButton.Text = "<";
            this.minusButton.UseVisualStyleBackColor = true;
            this.minusButton.Click += new System.EventHandler(this.minusButton_Click);
            // 
            // plusButton
            // 
            this.plusButton.Location = new System.Drawing.Point(471, 511);
            this.plusButton.Name = "plusButton";
            this.plusButton.Size = new System.Drawing.Size(20, 23);
            this.plusButton.TabIndex = 11;
            this.plusButton.Text = ">";
            this.plusButton.UseVisualStyleBackColor = true;
            this.plusButton.Click += new System.EventHandler(this.plusButton_Click);
            // 
            // outputClearButton
            // 
            this.outputClearButton.Location = new System.Drawing.Point(418, 26);
            this.outputClearButton.Name = "outputClearButton";
            this.outputClearButton.Size = new System.Drawing.Size(75, 23);
            this.outputClearButton.TabIndex = 10;
            this.outputClearButton.Text = "clear";
            this.outputClearButton.UseVisualStyleBackColor = true;
            this.outputClearButton.Click += new System.EventHandler(this.outputClearButton_Click);
            // 
            // receiveBox
            // 
            this.receiveBox.AcceptsReturn = true;
            this.receiveBox.AcceptsTab = true;
            this.receiveBox.Font = new System.Drawing.Font("Lucida Console", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(238)));
            this.receiveBox.Location = new System.Drawing.Point(0, 48);
            this.receiveBox.Multiline = true;
            this.receiveBox.Name = "receiveBox";
            this.receiveBox.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.receiveBox.Size = new System.Drawing.Size(493, 457);
            this.receiveBox.TabIndex = 1;
            this.receiveBox.WordWrap = false;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(214, 3);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(53, 13);
            this.label2.TabIndex = 0;
            this.label2.Text = "RECEIVE";
            // 
            // inputOpenFileDialog
            // 
            this.inputOpenFileDialog.Filter = "JSON files (*.JSN)|*.JSN";
            this.inputOpenFileDialog.FileOk += new System.ComponentModel.CancelEventHandler(this.openFileDialog1_FileOk);
            // 
            // timer
            // 
            this.timer.Enabled = true;
            this.timer.Tick += new System.EventHandler(this.timer_Tick);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.AutoSize = true;
            this.ClientSize = new System.Drawing.Size(1062, 812);
            this.Controls.Add(this.panel2);
            this.Controls.Add(this.sendPanel);
            this.Controls.Add(this.panel1);
            this.Name = "Form1";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Desktop Communication with Arduino System";
            this.Shown += new System.EventHandler(this.Form1_Shown);
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            this.sendPanel.ResumeLayout(false);
            this.sendPanel.PerformLayout();
            this.panel2.ResumeLayout(false);
            this.panel2.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox sendBox;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Label ipText;
        private System.Windows.Forms.Label portText;
        private System.Windows.Forms.TextBox ipBox;
        private System.Windows.Forms.TextBox remotePortBox;
        private System.Windows.Forms.Panel sendPanel;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Panel panel2;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Button inputClearButton;
        private System.Windows.Forms.Button statusButton;
        private System.Windows.Forms.Button sendButton;
        private System.Windows.Forms.Button helpButton;
        private System.Windows.Forms.Button button3;
        private System.Windows.Forms.Button button2;
        private System.Windows.Forms.Button listButton;
        private System.Windows.Forms.Button outputClearButton;
        private System.Windows.Forms.TextBox receiveBox;
        private System.Windows.Forms.OpenFileDialog inputOpenFileDialog;
        private System.Windows.Forms.Button plusButton;
        private System.Windows.Forms.TextBox packetNumBox;
        private System.Windows.Forms.Button minusButton;
        private System.Windows.Forms.Button inputSaveButton;
        private System.Windows.Forms.Button inputLoadButton;
        private System.Windows.Forms.Button clearAllButton;
        private System.Windows.Forms.Button outputSaveButton;
        private System.Windows.Forms.SaveFileDialog inputSaveFileDialog;
        private System.Windows.Forms.SaveFileDialog outputSaveFileDialog;
        private System.Windows.Forms.Timer timer;
        private System.Windows.Forms.Label localPortText;
        private System.Windows.Forms.TextBox localIPBox;
        private System.Windows.Forms.Label localIPText;
        private System.Windows.Forms.TextBox localPortBox;
        private System.Windows.Forms.Button dlBasicButton;
        private System.Windows.Forms.Button dlIOButton;
    }
}

