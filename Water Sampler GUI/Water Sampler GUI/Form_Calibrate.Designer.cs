namespace Water_Sampler_GUI
{
    partial class Form_Calibrate
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
            this.btnExit = new System.Windows.Forms.Button();
            this.panel3 = new System.Windows.Forms.Panel();
            this.btnRead = new System.Windows.Forms.Button();
            this.label2 = new System.Windows.Forms.Label();
            this.btnSave = new System.Windows.Forms.Button();
            this.cmbxSensor = new System.Windows.Forms.ComboBox();
            this.panel2 = new System.Windows.Forms.Panel();
            this.label1 = new System.Windows.Forms.Label();
            this.panel1 = new System.Windows.Forms.Panel();
            this.tbConsoleCalibrate = new System.Windows.Forms.TextBox();
            this.panel4 = new System.Windows.Forms.Panel();
            this.panel5 = new System.Windows.Forms.Panel();
            this.label6 = new System.Windows.Forms.Label();
            this.tbCoefD = new System.Windows.Forms.TextBox();
            this.tbCoefC = new System.Windows.Forms.TextBox();
            this.tbCoefB = new System.Windows.Forms.TextBox();
            this.tbCoefA = new System.Windows.Forms.TextBox();
            this.label5 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.lblFormula = new System.Windows.Forms.Label();
            this.panel3.SuspendLayout();
            this.panel2.SuspendLayout();
            this.panel1.SuspendLayout();
            this.panel4.SuspendLayout();
            this.panel5.SuspendLayout();
            this.SuspendLayout();
            // 
            // btnExit
            // 
            this.btnExit.Font = new System.Drawing.Font("Arial Narrow", 13.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnExit.Location = new System.Drawing.Point(911, 306);
            this.btnExit.Name = "btnExit";
            this.btnExit.Size = new System.Drawing.Size(125, 40);
            this.btnExit.TabIndex = 7;
            this.btnExit.Text = "Exit";
            this.btnExit.UseVisualStyleBackColor = true;
            this.btnExit.Click += new System.EventHandler(this.btnSave_Click);
            // 
            // panel3
            // 
            this.panel3.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.panel3.Controls.Add(this.btnRead);
            this.panel3.Controls.Add(this.label2);
            this.panel3.Controls.Add(this.btnSave);
            this.panel3.Controls.Add(this.cmbxSensor);
            this.panel3.Location = new System.Drawing.Point(225, 15);
            this.panel3.Name = "panel3";
            this.panel3.Size = new System.Drawing.Size(669, 54);
            this.panel3.TabIndex = 10;
            // 
            // btnRead
            // 
            this.btnRead.Enabled = false;
            this.btnRead.Font = new System.Drawing.Font("Arial Narrow", 13.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnRead.Location = new System.Drawing.Point(384, 6);
            this.btnRead.Name = "btnRead";
            this.btnRead.Size = new System.Drawing.Size(125, 40);
            this.btnRead.TabIndex = 13;
            this.btnRead.Text = "Read Value";
            this.btnRead.UseVisualStyleBackColor = true;
            this.btnRead.Click += new System.EventHandler(this.btnRead_Click);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Font = new System.Drawing.Font("Arial Narrow", 14F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label2.Location = new System.Drawing.Point(10, 10);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(81, 29);
            this.label2.TabIndex = 10;
            this.label2.Text = "Sensor:";
            // 
            // btnSave
            // 
            this.btnSave.Enabled = false;
            this.btnSave.Font = new System.Drawing.Font("Arial Narrow", 13.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnSave.Location = new System.Drawing.Point(526, 6);
            this.btnSave.Name = "btnSave";
            this.btnSave.Size = new System.Drawing.Size(125, 40);
            this.btnSave.TabIndex = 12;
            this.btnSave.Text = "Save";
            this.btnSave.UseVisualStyleBackColor = true;
            this.btnSave.Click += new System.EventHandler(this.btnSave_Click_1);
            // 
            // cmbxSensor
            // 
            this.cmbxSensor.Font = new System.Drawing.Font("Arial", 14F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.cmbxSensor.FormattingEnabled = true;
            this.cmbxSensor.Items.AddRange(new object[] {
            "Temperature",
            "Turbidity"});
            this.cmbxSensor.Location = new System.Drawing.Point(95, 10);
            this.cmbxSensor.Name = "cmbxSensor";
            this.cmbxSensor.Size = new System.Drawing.Size(267, 34);
            this.cmbxSensor.TabIndex = 9;
            this.cmbxSensor.SelectedIndexChanged += new System.EventHandler(this.cmbxPort_SelectedIndexChanged);
            // 
            // panel2
            // 
            this.panel2.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.panel2.Controls.Add(this.label1);
            this.panel2.Location = new System.Drawing.Point(20, 12);
            this.panel2.Name = "panel2";
            this.panel2.Size = new System.Drawing.Size(1048, 70);
            this.panel2.TabIndex = 11;
            // 
            // label1
            // 
            this.label1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.label1.Font = new System.Drawing.Font("Arial Black", 18F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.Location = new System.Drawing.Point(10, 10);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(1026, 48);
            this.label1.TabIndex = 0;
            this.label1.Text = "Calibrate Sensors";
            this.label1.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // panel1
            // 
            this.panel1.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.panel1.Controls.Add(this.tbConsoleCalibrate);
            this.panel1.Controls.Add(this.panel4);
            this.panel1.Controls.Add(this.btnExit);
            this.panel1.Controls.Add(this.panel3);
            this.panel1.Location = new System.Drawing.Point(20, 100);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(1048, 362);
            this.panel1.TabIndex = 13;
            this.panel1.Paint += new System.Windows.Forms.PaintEventHandler(this.panel1_Paint);
            // 
            // tbConsoleCalibrate
            // 
            this.tbConsoleCalibrate.Font = new System.Drawing.Font("Arial", 14F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.tbConsoleCalibrate.Location = new System.Drawing.Point(390, 89);
            this.tbConsoleCalibrate.Multiline = true;
            this.tbConsoleCalibrate.Name = "tbConsoleCalibrate";
            this.tbConsoleCalibrate.ReadOnly = true;
            this.tbConsoleCalibrate.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.tbConsoleCalibrate.Size = new System.Drawing.Size(646, 209);
            this.tbConsoleCalibrate.TabIndex = 12;
            // 
            // panel4
            // 
            this.panel4.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.panel4.Controls.Add(this.panel5);
            this.panel4.Controls.Add(this.lblFormula);
            this.panel4.Location = new System.Drawing.Point(12, 89);
            this.panel4.Name = "panel4";
            this.panel4.Size = new System.Drawing.Size(372, 262);
            this.panel4.TabIndex = 11;
            // 
            // panel5
            // 
            this.panel5.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.panel5.Controls.Add(this.label6);
            this.panel5.Controls.Add(this.tbCoefD);
            this.panel5.Controls.Add(this.tbCoefC);
            this.panel5.Controls.Add(this.tbCoefB);
            this.panel5.Controls.Add(this.tbCoefA);
            this.panel5.Controls.Add(this.label5);
            this.panel5.Controls.Add(this.label3);
            this.panel5.Controls.Add(this.label4);
            this.panel5.Location = new System.Drawing.Point(11, 46);
            this.panel5.Name = "panel5";
            this.panel5.Size = new System.Drawing.Size(344, 210);
            this.panel5.TabIndex = 6;
            // 
            // label6
            // 
            this.label6.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.label6.Font = new System.Drawing.Font("Arial Narrow", 13.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label6.Location = new System.Drawing.Point(10, 166);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(89, 34);
            this.label6.TabIndex = 9;
            this.label6.Text = "Coef_D";
            this.label6.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // tbCoefD
            // 
            this.tbCoefD.Enabled = false;
            this.tbCoefD.Font = new System.Drawing.Font("Arial Narrow", 13.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.tbCoefD.Location = new System.Drawing.Point(123, 166);
            this.tbCoefD.Name = "tbCoefD";
            this.tbCoefD.Size = new System.Drawing.Size(194, 34);
            this.tbCoefD.TabIndex = 8;
            // 
            // tbCoefC
            // 
            this.tbCoefC.Enabled = false;
            this.tbCoefC.Font = new System.Drawing.Font("Arial Narrow", 13.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.tbCoefC.Location = new System.Drawing.Point(123, 116);
            this.tbCoefC.Name = "tbCoefC";
            this.tbCoefC.Size = new System.Drawing.Size(194, 34);
            this.tbCoefC.TabIndex = 7;
            // 
            // tbCoefB
            // 
            this.tbCoefB.Enabled = false;
            this.tbCoefB.Font = new System.Drawing.Font("Arial Narrow", 13.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.tbCoefB.Location = new System.Drawing.Point(123, 66);
            this.tbCoefB.Name = "tbCoefB";
            this.tbCoefB.Size = new System.Drawing.Size(194, 34);
            this.tbCoefB.TabIndex = 6;
            // 
            // tbCoefA
            // 
            this.tbCoefA.Enabled = false;
            this.tbCoefA.Font = new System.Drawing.Font("Arial Narrow", 13.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.tbCoefA.Location = new System.Drawing.Point(123, 16);
            this.tbCoefA.Name = "tbCoefA";
            this.tbCoefA.Size = new System.Drawing.Size(194, 34);
            this.tbCoefA.TabIndex = 5;
            // 
            // label5
            // 
            this.label5.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.label5.Font = new System.Drawing.Font("Arial Narrow", 13.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label5.Location = new System.Drawing.Point(10, 116);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(89, 34);
            this.label5.TabIndex = 4;
            this.label5.Text = "Coef_C";
            this.label5.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // label3
            // 
            this.label3.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.label3.Font = new System.Drawing.Font("Arial Narrow", 13.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label3.Location = new System.Drawing.Point(10, 16);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(89, 34);
            this.label3.TabIndex = 2;
            this.label3.Text = "Coef_A";
            this.label3.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // label4
            // 
            this.label4.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.label4.Font = new System.Drawing.Font("Arial Narrow", 13.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label4.Location = new System.Drawing.Point(10, 66);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(89, 34);
            this.label4.TabIndex = 3;
            this.label4.Text = "Coef_B";
            this.label4.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // lblFormula
            // 
            this.lblFormula.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.lblFormula.Font = new System.Drawing.Font("Arial Narrow", 13.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblFormula.Location = new System.Drawing.Point(6, 5);
            this.lblFormula.Name = "lblFormula";
            this.lblFormula.Size = new System.Drawing.Size(361, 34);
            this.lblFormula.TabIndex = 1;
            this.lblFormula.Text = "Formula: Y = Coef_A*x + Coef_B";
            this.lblFormula.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // Form_Calibrate
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1080, 473);
            this.Controls.Add(this.panel1);
            this.Controls.Add(this.panel2);
            this.Name = "Form_Calibrate";
            this.Text = "Calibrate";
            this.panel3.ResumeLayout(false);
            this.panel3.PerformLayout();
            this.panel2.ResumeLayout(false);
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            this.panel4.ResumeLayout(false);
            this.panel5.ResumeLayout(false);
            this.panel5.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button btnExit;
        private System.Windows.Forms.Panel panel3;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.ComboBox cmbxSensor;
        private System.Windows.Forms.Panel panel2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button btnSave;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Panel panel4;
        private System.Windows.Forms.Label lblFormula;
        private System.Windows.Forms.TextBox tbConsoleCalibrate;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox tbCoefA;
        private System.Windows.Forms.Panel panel5;
        private System.Windows.Forms.TextBox tbCoefC;
        private System.Windows.Forms.TextBox tbCoefB;
        private System.Windows.Forms.TextBox tbCoefD;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Button btnRead;
    }
}