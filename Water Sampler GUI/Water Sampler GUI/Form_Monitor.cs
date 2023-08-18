using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Threading;
using System.Windows.Forms;

namespace Water_Sampler_GUI
{
    public partial class Form_Monitor : Form
    {

        bool updateValues;
        private Form_Welcome _formWelcome;

        public Form_Monitor(Form_Welcome formWelcome)
        {
            InitializeComponent();
            updateValues = true;
            System.Threading.Timer timer = new System.Threading.Timer(UpdateCallback, null, 0, 1000);

            _formWelcome = formWelcome;

        }

        private void btnSave_Click(object sender, EventArgs e)
        {
            Form_Welcome.bConnected = true;
            this.Close();
        }

        void UpdateCallback(object state)
        {
            if (updateValues == true)
            {
                _formWelcome.SerialPortInstance.WriteLine("M" + "#" +"0"+ "#");
            }  
        }
    }
}
