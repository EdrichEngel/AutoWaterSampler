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
using static System.Net.Mime.MediaTypeNames;
using static System.Windows.Forms.VisualStyles.VisualStyleElement;

namespace Water_Sampler_GUI
{
    public partial class Form_Calibrate : Form
    {
        private string _receivedData;
        private bool _received = false;
        public static int waitForResponse = 10;
        private Form_Welcome _formWelcome;

        public Form_Calibrate(Form_Welcome formWelcome)
        {
            InitializeComponent();

            this.FormClosing += Form_Calibrate_FormClosing;
            tbCoefA.Text = "0";
            tbCoefB.Text = "0";
            tbCoefC.Text = "0";
            tbCoefD.Text = "0";
            lblFormula.Text = "Please Select Sensor";

            _formWelcome = formWelcome;


            

        }

        private void btnSave_Click(object sender, EventArgs e)
        {
            Form_Welcome.bConnected = true;
            _formWelcome.SerialPortInstance.DataReceived -= new SerialDataReceivedEventHandler(SerialPort_DataReceived);
            this.Close();
        }

        private void btnSave_Click_1(object sender, EventArgs e)
        {

            if (CheckDataFormat())
            {
                _formWelcome.SerialPortInstance.WriteLine("C" + "#" + cmbxSensor.SelectedIndex + "#" + tbCoefA.Text + "#" +
                    tbCoefB.Text + "#" + tbCoefC.Text + "#" + tbCoefD.Text + "#");
                TextBoxWriteLine("Successfully Saved");
            }
            else
            {

                TextBoxWriteLine("Save Unsuccessfull");
            }


        }

        private void cmbxPort_SelectedIndexChanged(object sender, EventArgs e)
        {
            switch (cmbxSensor.SelectedIndex)
            {
                case -1:
                    // Empty index selected
                    lblFormula.Text = "Please Select Sensor";
                    btnSave.Enabled = false;
                    btnRead.Enabled = false;
                    tbCoefA.Enabled = false;
                    tbCoefB.Enabled = false;
                    tbCoefC.Enabled = false;
                    tbCoefD.Enabled = false;

                    tbCoefA.Text = "0";
                    tbCoefB.Text = "0";
                    tbCoefC.Text = "0";
                    tbCoefD.Text = "0";

                    //Read value form micro controller.


                    break;

                case 0:
                    // Temperature
                    lblFormula.Text = "Formula: ";
                    btnSave.Enabled = true;
                    btnRead.Enabled = true;
                    tbCoefA.Enabled = true;
                    tbCoefB.Enabled = true;
                    tbCoefC.Enabled = false;
                    tbCoefD.Enabled = false;
                    TextBoxWriteLine("Temperature Sensor Selected");

                    ReadCoefValues();

                    //Read value form micro controller.

                    break;

                case 1:
                    // Turbidity
                    lblFormula.Text = "Formula: ";
                    btnSave.Enabled = true;
                    btnRead.Enabled = true;
                    tbCoefA.Enabled = true;
                    tbCoefB.Enabled = true;
                    tbCoefC.Enabled = true;
                    tbCoefD.Enabled = false;
                    TextBoxWriteLine("Turbidity Sensor Selected");

                    ReadCoefValues();

                    //Read value form micro controller.

                    break;
            }
        }

        private void SerialPort_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            SerialPort serialPort = (SerialPort)sender;
           _receivedData = serialPort.ReadLine();
            
            _received = true;



            //TextBoxWriteLine(_receivedData);

        }


        delegate void SetTextCallback(string text);

        private void TextBoxWriteLine(string data)
        {

            string currentDateTime = DateTime.Now.ToString("HH:mm:ss");

            // Used the following form to fix an error.
            //https://stackoverflow.com/questions/10775367/cross-thread-operation-not-valid-control-textbox1-accessed-from-a-thread-othe

            if (this.tbConsoleCalibrate.InvokeRequired)
            {
                SetTextCallback d = new SetTextCallback(TextBoxWriteLine);
                this.Invoke(d, new object[] { data });
            }
            else
            {
                this.tbConsoleCalibrate.AppendText(currentDateTime + " | " + data);
                this.tbConsoleCalibrate.AppendText(Environment.NewLine);
            }



        }

        private bool CheckDataFormat(){

    

            if (tbCoefA.Text == "")
            {
                tbCoefA.Text = "0";
            }
            if (tbCoefB.Text == "")
            {
                tbCoefB.Text = "0";
            }
            if (tbCoefC.Text == "")
            {
                tbCoefC.Text = "0";
            }
            if (tbCoefD.Text == "")
            {
                tbCoefD.Text = "0";
            }

        
            if ((!float.TryParse(tbCoefA.Text, out _))) {
                TextBoxWriteLine("Invalid Value for Coef_A");
                return false;
            }
            if ((!float.TryParse(tbCoefB.Text, out _)))
            {
                TextBoxWriteLine("Invalid Value for Coef_B");
                return false;
            }
            if ((!float.TryParse(tbCoefC.Text, out _)))
            {
                TextBoxWriteLine("Invalid Value for Coef_C");
                return false;
            }
            if ((!float.TryParse(tbCoefD.Text, out _)))
            {
                TextBoxWriteLine("Invalid Value for Coef_D");
                return false;
            }

            return true;

            }

        public void ReadCoefValues()
        {


             TextBoxWriteLine("Reading Coefficients From System");
            _formWelcome.SerialPortInstance.WriteLine("R"+"#"+cmbxSensor.SelectedIndex);




            _receivedData = null;
            _formWelcome.SerialPortInstance.DataReceived += SerialPort_DataReceived;

            // Wait for input for 3 seconds
            bool success = SpinWait.SpinUntil(() => _receivedData != null, TimeSpan.FromSeconds(waitForResponse));

            _formWelcome.SerialPortInstance.DataReceived -= SerialPort_DataReceived;

            if (success)
            {

                DecodeString();
                TextBoxWriteLine("Read Successful");

            }
            else
            {
                TextBoxWriteLine("No Repsonse From device");
                MessageBox.Show("No Resoponse From Device.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);

            }

        }

        public void DecodeString()
        {
            int stringLength = _receivedData.Length;
            int nextPos = 0;
            string tempOutput;

            // fix issue here. there is a bug on line 251 to 260

            string temp = _receivedData.Substring(3, (stringLength-3));

            if (_receivedData[0] == 'R' && _receivedData[1] == 'R')
            {
                
                    nextPos = temp.IndexOf('#');
                    tbCoefA.Text = temp.Substring(0, nextPos);
                    temp = temp.Substring(nextPos+1, (temp.Length - nextPos-1)); 

                    nextPos = temp.IndexOf('#');
                    tbCoefB.Text = temp.Substring(0, nextPos);
                    temp = temp.Substring(nextPos + 1, (temp.Length - nextPos-1)); 

                    nextPos = temp.IndexOf('#');
                    tbCoefC.Text = temp.Substring(0, nextPos);
                    temp = temp.Substring(nextPos + 1, (temp.Length - nextPos-1)); 

                    nextPos = temp.IndexOf('#');
                    tbCoefD.Text = temp.Substring(0, nextPos);

                

            } 
            if (_receivedData[0] == 'S' && _receivedData[1] == 'R')
            {
                nextPos = temp.IndexOf('#');
                tempOutput = " \t " + (temp.Substring(0, nextPos));
                temp = temp.Substring(nextPos + 1, (temp.Length - nextPos - 1));

                tempOutput += " \t | \t ";

                nextPos = temp.IndexOf('#');
                tempOutput += (temp.Substring(0, nextPos));

                TextBoxWriteLine(tempOutput);
                //temp = temp.Substring(nextPos + 1, (temp.Length - nextPos - 1));
            }
           

        }

        private void Form_Calibrate_FormClosing(object sender, FormClosingEventArgs e)
        {
            _formWelcome.SerialPortInstance.DataReceived -= new SerialDataReceivedEventHandler(SerialPort_DataReceived);
        }

        private void btnRead_Click(object sender, EventArgs e)
        {
            TextBoxWriteLine("Reading Value From " + cmbxSensor.Items[cmbxSensor.SelectedIndex] + " Sensor");

            _formWelcome.SerialPortInstance.DiscardInBuffer();

            _formWelcome.SerialPortInstance.WriteLine("S" + "#" + cmbxSensor.SelectedIndex);




            _receivedData = null;
            _formWelcome.SerialPortInstance.DataReceived += SerialPort_DataReceived;

            // Wait for input for 3 seconds
            bool success = SpinWait.SpinUntil(() => _receivedData != null, TimeSpan.FromSeconds(waitForResponse));

            _formWelcome.SerialPortInstance.DataReceived -= SerialPort_DataReceived;

            if (success)
            {

                TextBoxWriteLine(" \t " + "Raw \t | \t Processed");
                DecodeString();

            }
            else
            {
                TextBoxWriteLine("No Repsonse From device");
                MessageBox.Show("No Resoponse From Device.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);

            }
        }

        private void panel1_Paint(object sender, PaintEventArgs e)
        {

        }
    }
}
