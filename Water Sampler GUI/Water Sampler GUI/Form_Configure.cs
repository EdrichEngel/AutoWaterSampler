using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Water_Sampler_GUI
{
    public partial class Form_Configure : Form
    {

        private Form_Welcome _formWelcome;
        private string _receivedData;
        private bool _received = false;

        public Form_Configure(Form_Welcome formWelcome)
        {
            InitializeComponent();
            _formWelcome = formWelcome;
            pullConfig();
        }

        private void btnSave_Click(object sender, EventArgs e)
        {
            Form_Welcome.bConnected = true;

            this.Close();
        }
        private void SerialPort_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            SerialPort serialPort = (SerialPort)sender;
            _receivedData = serialPort.ReadLine();

            _received = true;



            //TextBoxWriteLine(_receivedData);

        }

        private void pullConfig()
        {
            _formWelcome.SerialPortInstance.WriteLine("CR#");



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
                
                MessageBox.Show("No Resoponse From Device.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);


                btnSave.PerformClick();



            }
        }

        public void DecodeString()
        {
            int stringLength = _receivedData.Length;
            int nextPos = 0;
            //string tempOutput;


            string temp = _receivedData.Substring(3, (stringLength - 3));

            if (_receivedData[0] == 'C' && _receivedData[1] == 'W')
            {
                

                nextPos = temp.IndexOf('#');
                tbFileName.Text = temp.Substring(0, nextPos);
                temp = temp.Substring(nextPos + 1, (temp.Length - nextPos - 1));

                nextPos = temp.IndexOf('#');
                tbSampleDate.Text = temp.Substring(0, nextPos);
                temp = temp.Substring(nextPos + 1, (temp.Length - nextPos - 1));

                nextPos = temp.IndexOf('#');
                tbLocation.Text = temp.Substring(0, nextPos);
                temp = temp.Substring(nextPos + 1, (temp.Length - nextPos - 1));

                nextPos = temp.IndexOf('#');
                tbCount.Text = temp.Substring(0, nextPos);
                temp = temp.Substring(nextPos + 1, (temp.Length - nextPos - 1));

                nextPos = temp.IndexOf('#');
                tbInterval.Text = temp.Substring(0, nextPos);
                //temp = temp.Substring(nextPos + 1, (temp.Length - nextPos - 1));

            }

        }

        private void btnPull_Click(object sender, EventArgs e)
        {
            _formWelcome.SerialPortInstance.DiscardInBuffer();
            pullConfig();
        }

        private void btnExit_Click(object sender, EventArgs e)
        {

            btnSave.PerformClick();

            Form_Welcome.bConnected = true;
            this.Close();
        }


        
        private bool ValidFileName(string filename)
        {
            string[] invalidCharArray = { "#", "%", "&", "{", "}", "\\","<", ">", "*", "?", "$", "!", "'", "\"",":","@", "+", "`", "|", "="};
            int invalCount = 0;

            for (int i = 0; i < invalidCharArray.Length; i++)
            {
                if (filename.IndexOf(invalidCharArray[i]) != -1)
                {
                   // MessageBox.Show(invalidCharArray[i], "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);

                    invalCount++;
                }
            }

           
            if (invalCount == 0)
            {
                return true;
            }
            else
            {
                MessageBox.Show("Incorrect data format: File name. Diverse Error Count: " + invalCount.ToString(), "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);

                return false;
            }

         
        }
        private bool hasTxtExtension(string input)
        {
            string targetExtension = ".txt";

            if (input.Length < targetExtension.Length)
                return false;

            string lastCharacters = input.Substring(input.Length - targetExtension.Length);
        
            
            if(lastCharacters.IndexOf(".txt") == 0)
            {
                return true;
            }
            else
            {
                return false;
            }

        }

        private bool CheckDataFormat()
        {

            if (!ValidFileName(tbFileName.Text))
            {
               
                //MessageBox.Show("Incorrect data format: File name", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return false;
            }

            if (tbFileName.Text == "" || tbFileName.Text == " ")
            {
                MessageBox.Show("Incorrect data format: File name", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return false;
            }
            if (!hasTxtExtension(tbFileName.Text))
            {
                MessageBox.Show("Incorrect data format: File extention must be .txt", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return false;
            }

            if(tbSampleDate.Text.IndexOf("#") != -1)
            {
                MessageBox.Show("Incorrect data format: Date", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return false;
            }

            if (tbLocation.Text.IndexOf("#") != -1)
            {
                MessageBox.Show("Incorrect data format: Location", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return false;
            }

            if ((!int.TryParse(tbCount.Text, out _)))
            {
                MessageBox.Show("Incorrect data format: Count", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return false;
            }

            if ((!int.TryParse(tbInterval.Text, out _)))
            {
                MessageBox.Show("Incorrect data format: Interval", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return false;
            }

            return true;
        }

        private void btnSave_Click_1(object sender, EventArgs e)
        {
            if (CheckDataFormat())
            {
                _formWelcome.SerialPortInstance.WriteLine("CS" + "#" + tbFileName.Text + "#" + tbSampleDate.Text + "#" +
                    tbLocation.Text + "#" + tbCount.Text + "#" + tbInterval.Text + "#");
               
            }
            else
            {

                //MessageBox.Show("Incorrect data format.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void btnDefault_Click(object sender, EventArgs e)
        {
            var dateAndTime = DateTime.Now;
            var date = dateAndTime.Date;

            tbFileName.Text = "Default.txt";
            tbSampleDate.Text = DateTime.Now.ToString("dd/MM/yyyy");
            tbLocation.Text = "N/A";
            tbCount.Text = "4";
            tbInterval.Text = "600";
           
        }
    }
}
