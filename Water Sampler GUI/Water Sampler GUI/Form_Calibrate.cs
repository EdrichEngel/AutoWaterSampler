﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Water_Sampler_GUI
{
    public partial class Form_Calibrate : Form
    {
        public Form_Calibrate()
        {
            InitializeComponent();
        }

        private void btnSave_Click(object sender, EventArgs e)
        {
            Form_Welcome.bConnected = true;

            this.Close();
        }
    }
}