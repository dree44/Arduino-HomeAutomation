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
            this.sendBox = new System.Windows.Forms.TextBox();
            this.panel1 = new System.Windows.Forms.Panel();
            this.ipText = new System.Windows.Forms.Label();
            this.macText = new System.Windows.Forms.Label();
            this.ipBox = new System.Windows.Forms.TextBox();
            this.macBox = new System.Windows.Forms.TextBox();
            this.sendPanel = new System.Windows.Forms.Panel();
            this.button11 = new System.Windows.Forms.Button();
            this.button10 = new System.Windows.Forms.Button();
            this.button6 = new System.Windows.Forms.Button();
            this.button5 = new System.Windows.Forms.Button();
            this.sendButton = new System.Windows.Forms.Button();
            this.button4 = new System.Windows.Forms.Button();
            this.button3 = new System.Windows.Forms.Button();
            this.button2 = new System.Windows.Forms.Button();
            this.button1 = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.panel2 = new System.Windows.Forms.Panel();
            this.clearAllButton = new System.Windows.Forms.Button();
            this.button12 = new System.Windows.Forms.Button();
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.button9 = new System.Windows.Forms.Button();
            this.button8 = new System.Windows.Forms.Button();
            this.button7 = new System.Windows.Forms.Button();
            this.receiveBox = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.openFileDialog1 = new System.Windows.Forms.OpenFileDialog();
            this.panel1.SuspendLayout();
            this.sendPanel.SuspendLayout();
            this.panel2.SuspendLayout();
            this.SuspendLayout();
            // 
            // sendBox
            // 
            this.sendBox.Location = new System.Drawing.Point(3, 48);
            this.sendBox.Multiline = true;
            this.sendBox.Name = "sendBox";
            this.sendBox.Size = new System.Drawing.Size(522, 457);
            this.sendBox.TabIndex = 0;
            // 
            // panel1
            // 
            this.panel1.AutoSize = true;
            this.panel1.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.panel1.Controls.Add(this.ipText);
            this.panel1.Controls.Add(this.macText);
            this.panel1.Controls.Add(this.ipBox);
            this.panel1.Controls.Add(this.macBox);
            this.panel1.Location = new System.Drawing.Point(16, 13);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(251, 56);
            this.panel1.TabIndex = 1;
            // 
            // ipText
            // 
            this.ipText.AutoSize = true;
            this.ipText.Location = new System.Drawing.Point(3, 32);
            this.ipText.Name = "ipText";
            this.ipText.Size = new System.Drawing.Size(98, 13);
            this.ipText.TabIndex = 3;
            this.ipText.Text = "Remote IP Address";
            // 
            // macText
            // 
            this.macText.AutoSize = true;
            this.macText.Location = new System.Drawing.Point(3, 6);
            this.macText.Name = "macText";
            this.macText.Size = new System.Drawing.Size(111, 13);
            this.macText.TabIndex = 2;
            this.macText.Text = "Remote MAC Address";
            // 
            // ipBox
            // 
            this.ipBox.Location = new System.Drawing.Point(119, 29);
            this.ipBox.Name = "ipBox";
            this.ipBox.Size = new System.Drawing.Size(125, 20);
            this.ipBox.TabIndex = 1;
            // 
            // macBox
            // 
            this.macBox.Location = new System.Drawing.Point(119, 3);
            this.macBox.Name = "macBox";
            this.macBox.Size = new System.Drawing.Size(125, 20);
            this.macBox.TabIndex = 0;
            // 
            // sendPanel
            // 
            this.sendPanel.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.sendPanel.Controls.Add(this.button11);
            this.sendPanel.Controls.Add(this.button10);
            this.sendPanel.Controls.Add(this.button6);
            this.sendPanel.Controls.Add(this.button5);
            this.sendPanel.Controls.Add(this.sendButton);
            this.sendPanel.Controls.Add(this.button4);
            this.sendPanel.Controls.Add(this.button3);
            this.sendPanel.Controls.Add(this.button2);
            this.sendPanel.Controls.Add(this.button1);
            this.sendPanel.Controls.Add(this.label1);
            this.sendPanel.Controls.Add(this.sendBox);
            this.sendPanel.Location = new System.Drawing.Point(17, 86);
            this.sendPanel.Name = "sendPanel";
            this.sendPanel.Size = new System.Drawing.Size(530, 690);
            this.sendPanel.TabIndex = 2;
            // 
            // button11
            // 
            this.button11.Location = new System.Drawing.Point(84, 26);
            this.button11.Name = "button11";
            this.button11.Size = new System.Drawing.Size(75, 23);
            this.button11.TabIndex = 11;
            this.button11.Text = "Save...";
            this.button11.UseVisualStyleBackColor = true;
            // 
            // button10
            // 
            this.button10.Location = new System.Drawing.Point(3, 26);
            this.button10.Name = "button10";
            this.button10.Size = new System.Drawing.Size(75, 23);
            this.button10.TabIndex = 10;
            this.button10.Text = "Load...";
            this.button10.UseVisualStyleBackColor = true;
            // 
            // button6
            // 
            this.button6.Location = new System.Drawing.Point(450, 26);
            this.button6.Name = "button6";
            this.button6.Size = new System.Drawing.Size(75, 23);
            this.button6.TabIndex = 9;
            this.button6.Text = "clear";
            this.button6.UseVisualStyleBackColor = true;
            // 
            // button5
            // 
            this.button5.Location = new System.Drawing.Point(286, 511);
            this.button5.Name = "button5";
            this.button5.Size = new System.Drawing.Size(75, 23);
            this.button5.TabIndex = 8;
            this.button5.Text = "status";
            this.button5.UseVisualStyleBackColor = true;
            // 
            // sendButton
            // 
            this.sendButton.Location = new System.Drawing.Point(447, 511);
            this.sendButton.Name = "sendButton";
            this.sendButton.Size = new System.Drawing.Size(75, 48);
            this.sendButton.TabIndex = 7;
            this.sendButton.Text = "Send";
            this.sendButton.UseVisualStyleBackColor = true;
            // 
            // button4
            // 
            this.button4.Location = new System.Drawing.Point(205, 536);
            this.button4.Name = "button4";
            this.button4.Size = new System.Drawing.Size(75, 23);
            this.button4.TabIndex = 6;
            this.button4.Text = "help";
            this.button4.UseVisualStyleBackColor = true;
            // 
            // button3
            // 
            this.button3.Location = new System.Drawing.Point(104, 511);
            this.button3.Name = "button3";
            this.button3.Size = new System.Drawing.Size(95, 48);
            this.button3.TabIndex = 5;
            this.button3.Text = "Upload basic\r\nBASIC.JSN";
            this.button3.UseVisualStyleBackColor = true;
            // 
            // button2
            // 
            this.button2.Location = new System.Drawing.Point(3, 511);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(95, 48);
            this.button2.TabIndex = 4;
            this.button2.Text = "Upload I/O set\r\nSETTING.JSN";
            this.button2.UseVisualStyleBackColor = true;
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(205, 511);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(75, 23);
            this.button1.TabIndex = 3;
            this.button1.Text = "list IO";
            this.button1.UseVisualStyleBackColor = true;
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
            this.panel2.Controls.Add(this.button12);
            this.panel2.Controls.Add(this.textBox1);
            this.panel2.Controls.Add(this.button9);
            this.panel2.Controls.Add(this.button8);
            this.panel2.Controls.Add(this.button7);
            this.panel2.Controls.Add(this.receiveBox);
            this.panel2.Controls.Add(this.label2);
            this.panel2.Location = new System.Drawing.Point(552, 86);
            this.panel2.Name = "panel2";
            this.panel2.Size = new System.Drawing.Size(502, 690);
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
            // button12
            // 
            this.button12.Location = new System.Drawing.Point(3, 27);
            this.button12.Name = "button12";
            this.button12.Size = new System.Drawing.Size(75, 23);
            this.button12.TabIndex = 12;
            this.button12.Text = "Save...";
            this.button12.UseVisualStyleBackColor = true;
            // 
            // textBox1
            // 
            this.textBox1.Location = new System.Drawing.Point(395, 513);
            this.textBox1.Name = "textBox1";
            this.textBox1.Size = new System.Drawing.Size(77, 20);
            this.textBox1.TabIndex = 13;
            // 
            // button9
            // 
            this.button9.Location = new System.Drawing.Point(376, 511);
            this.button9.Name = "button9";
            this.button9.Size = new System.Drawing.Size(20, 23);
            this.button9.TabIndex = 12;
            this.button9.Text = "<";
            this.button9.UseVisualStyleBackColor = true;
            // 
            // button8
            // 
            this.button8.Location = new System.Drawing.Point(471, 511);
            this.button8.Name = "button8";
            this.button8.Size = new System.Drawing.Size(20, 23);
            this.button8.TabIndex = 11;
            this.button8.Text = ">";
            this.button8.UseVisualStyleBackColor = true;
            // 
            // button7
            // 
            this.button7.Location = new System.Drawing.Point(418, 26);
            this.button7.Name = "button7";
            this.button7.Size = new System.Drawing.Size(75, 23);
            this.button7.TabIndex = 10;
            this.button7.Text = "clear";
            this.button7.UseVisualStyleBackColor = true;
            // 
            // receiveBox
            // 
            this.receiveBox.Location = new System.Drawing.Point(0, 48);
            this.receiveBox.Multiline = true;
            this.receiveBox.Name = "receiveBox";
            this.receiveBox.Size = new System.Drawing.Size(493, 457);
            this.receiveBox.TabIndex = 1;
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
            // openFileDialog1
            // 
            this.openFileDialog1.FileName = "openFileDialog1";
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
        private System.Windows.Forms.Label macText;
        private System.Windows.Forms.TextBox ipBox;
        private System.Windows.Forms.TextBox macBox;
        private System.Windows.Forms.Panel sendPanel;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Panel panel2;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Button button6;
        private System.Windows.Forms.Button button5;
        private System.Windows.Forms.Button sendButton;
        private System.Windows.Forms.Button button4;
        private System.Windows.Forms.Button button3;
        private System.Windows.Forms.Button button2;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.Button button7;
        private System.Windows.Forms.TextBox receiveBox;
        private System.Windows.Forms.OpenFileDialog openFileDialog1;
        private System.Windows.Forms.Button button8;
        private System.Windows.Forms.TextBox textBox1;
        private System.Windows.Forms.Button button9;
        private System.Windows.Forms.Button button11;
        private System.Windows.Forms.Button button10;
        private System.Windows.Forms.Button clearAllButton;
        private System.Windows.Forms.Button button12;
    }
}

