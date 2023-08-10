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
    public partial class Form_Welcome : Form
    {
        public static bool bConnected;

        public Form_Welcome()
        {
            InitializeComponent();
            this.FormClosing += Form_Welcome_FormClosing;

            bConnected = false;
        }

        private void button1_Click(object sender, EventArgs e)
        {

        }

        private void label4_Click(object sender, EventArgs e)
        {

        }

        private void btnExit_Click(object sender, EventArgs e)
        {
            Application.Exit();
        }

        private void btnConnect_Click(object sender, EventArgs e)
        {
            /*  Form_Connect connectForm = new Form_Connect();

              this.Hide();

              connectForm.Show();

              */


            Hide();
            Form_Connect connectForm = new Form_Connect();
            connectForm.ShowDialog();
            connectForm = null;
            Show();

            if (bConnected == true)
            {
                //MessageBox.Show("True", "TESTING");
                btnCalibrate.Enabled = true;
                btnMonitor.Enabled = true;
                btnConfigure.Enabled = true;

            } else
            {
                //MessageBox.Show("False", "TESTING");
                btnCalibrate.Enabled = false;
                btnMonitor.Enabled = false;
                btnConfigure.Enabled = false;
            }

            

        }

        private void Form_Welcome_Load(object sender, EventArgs e)
        {

        }

        private void Form_Welcome_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (MessageBox.Show("Are you sure you want to close the form?", "Confirmation", MessageBoxButtons.YesNo) == DialogResult.No)
            {
                e.Cancel = true; // Cancel the form closing
            }
        }

        private void btnCalibrate_Click(object sender, EventArgs e)
        {
            Hide();
            Form_Calibrate calibrateForm = new Form_Calibrate();
            calibrateForm.ShowDialog();
            calibrateForm = null;
            Show();
        }

        private void btnMonitor_Click(object sender, EventArgs e)
        {
            Hide();
            Form_Monitor monitorForm = new Form_Monitor();
            monitorForm.ShowDialog();
            monitorForm = null;
            Show();
        }

        private void btnConfigure_Click(object sender, EventArgs e)
        {
            Hide();
            Form_Configure configureForm = new Form_Configure();
            configureForm.ShowDialog();
            configureForm = null;
            Show();
        }

        private void label1_Click(object sender, EventArgs e)
        {

        }
    }
}