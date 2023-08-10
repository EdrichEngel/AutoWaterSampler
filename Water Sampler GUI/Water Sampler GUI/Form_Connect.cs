using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using static System.ActivationContext;

namespace Water_Sampler_GUI
{
    public partial class Form_Connect : Form
    {

     
        public Form_Connect()
        {
            InitializeComponent();


            this.FormClosing += Form_Connect_FormClosing;


        }

        private void btnSave_Click(object sender, EventArgs e)
        {
            Form_Welcome.bConnected = true;

            this.Close();
        }

        private void Form_Connect_FormClosing(Object sender, FormClosingEventArgs e)
        {
          
        }
    }
}
