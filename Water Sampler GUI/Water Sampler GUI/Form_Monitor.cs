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

using System.IO.Ports;

namespace Water_Sampler_GUI
{
    public partial class Form_Monitor : Form
    {

       

        private string _receivedData;
        private bool _received = false;
        bool updateValues;
        private Form_Welcome _formWelcome;

        public Form_Monitor(Form_Welcome formWelcome)
        {
            InitializeComponent();
            _formWelcome = formWelcome;
            updateValues = true;
            timerTickCode();
            timer1.Interval = 10000;
            timer1.Enabled = true;

        }

        private void btnSave_Click(object sender, EventArgs e)
        {
            timer1.Enabled = false;
            Form_Welcome.bConnected = true;
            this.Close();
        }


        private void timerTickCode()
        {
            _formWelcome.SerialPortInstance.DiscardInBuffer();

            _formWelcome.SerialPortInstance.WriteLine("MR#");



            _receivedData = null;
            _formWelcome.SerialPortInstance.DataReceived += SerialPort_DataReceived;

            // Wait for input for 3 seconds
            bool success = SpinWait.SpinUntil(() => _receivedData != null, TimeSpan.FromSeconds(3));

            _formWelcome.SerialPortInstance.DataReceived -= SerialPort_DataReceived;

            if (success)
            {

                DecodeString();

            }
            else
            {
                timer1.Enabled = false;
                MessageBox.Show("No Resoponse From Device.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);


                btnSave.PerformClick();



            }

        }


        private void timer1_Tick(object sender, EventArgs e)
        {

            timerTickCode();



        }


        private void SerialPort_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            SerialPort serialPort = (SerialPort)sender;
            _receivedData = serialPort.ReadLine();

            _received = true;



            //TextBoxWriteLine(_receivedData);

        }

        public void DecodeString()
        {
            int stringLength = _receivedData.Length;
            int nextPos = 0;
            //string tempOutput;

       

            string temp = _receivedData.Substring(3, (stringLength - 3));

            if (_receivedData[0] == 'M' && _receivedData[1] == 'W')
            {

                nextPos = temp.IndexOf('#');
                lblDate.Text = temp.Substring(0, nextPos);
                temp = temp.Substring(nextPos + 1, (temp.Length - nextPos - 1));

                nextPos = temp.IndexOf('#');
                lblBattery.Text = temp.Substring(0, nextPos);
                temp = temp.Substring(nextPos + 1, (temp.Length - nextPos - 1));

                nextPos = temp.IndexOf('#');
                lblSDCard.Text = temp.Substring(0, nextPos);
                temp = temp.Substring(nextPos + 1, (temp.Length - nextPos - 1));

                nextPos = temp.IndexOf('#');
                lblSDCap.Text = temp.Substring(0, nextPos);
                temp = temp.Substring(nextPos + 1, (temp.Length - nextPos - 1));

                nextPos = temp.IndexOf('#');
                lblSDUsed.Text = temp.Substring(0, nextPos);
                temp = temp.Substring(nextPos + 1, (temp.Length - nextPos - 1));

                nextPos = temp.IndexOf('#');
                lblSDUsedPercentage.Text = temp.Substring(0, nextPos);
                temp = temp.Substring(nextPos + 1, (temp.Length - nextPos - 1));

                nextPos = temp.IndexOf('#');
                lblTemp.Text = temp.Substring(0, nextPos);
                temp = temp.Substring(nextPos + 1, (temp.Length - nextPos - 1));

                nextPos = temp.IndexOf('#');
                lblTurb.Text = temp.Substring(0, nextPos);
                //temp = temp.Substring(nextPos + 1, (temp.Length - nextPos - 1));


            }
            
        }
    }
}
