using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using static System.Net.Mime.MediaTypeNames;

namespace Water_Sampler_GUI
{
    public partial class Form_Calibrate : Form
    {
        private string _receivedData;
        private bool _received = false;

        private Form_Welcome _formWelcome;

        public Form_Calibrate(Form_Welcome formWelcome)
        {
            InitializeComponent();
            lblFormula.Text = "Please Select Sensor";

            _formWelcome = formWelcome;

            _formWelcome.SerialPortInstance.DataReceived += new SerialDataReceivedEventHandler(SerialPort_DataReceived);

        }

        private void btnSave_Click(object sender, EventArgs e)
        {
            Form_Welcome.bConnected = true;

            this.Close();
        }

        private void btnSave_Click_1(object sender, EventArgs e)
        {

            TextBoxWriteLine("Successfully Saved");
        }

        private void cmbxPort_SelectedIndexChanged(object sender, EventArgs e)
        {
            switch (cmbxSensor.SelectedIndex)
            {
                case -1:
                    // Empty index selected
                    lblFormula.Text = "Please Select Sensor";
                    btnSave.Enabled = false;
                    tbCoefA.Enabled = false;
                    tbCoefB.Enabled = false;
                    tbCoefC.Enabled = false;
                    tbCoefD.Enabled = false;
                    break;

                case 0:
                    // Temperature
                    lblFormula.Text = "Formula: ";
                    btnSave.Enabled = true;
                    tbCoefA.Enabled = true;
                    tbCoefB.Enabled = true;
                    tbCoefC.Enabled = false;
                    tbCoefD.Enabled = false;
                    TextBoxWriteLine("Temperature Sensor Selected");

                    break;

                case 1:
                    // Turbidity
                    lblFormula.Text = "Formula: ";
                    btnSave.Enabled = true;
                    tbCoefA.Enabled = true;
                    tbCoefB.Enabled = true;
                    tbCoefC.Enabled = true;
                    tbCoefD.Enabled = false;
                    TextBoxWriteLine("Turbidity Sensor Selected");

                    break;
            }
        }

        private void SerialPort_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            SerialPort serialPort = (SerialPort)sender;
            _receivedData = serialPort.ReadLine();
            _received = true;



            TextBoxWriteLine(_receivedData);

        }


        delegate void SetTextCallback(string text);

        private void TextBoxWriteLine(string data)
        {

            string currentDateTime = DateTime.Now.ToString("HH:mm:ss");

            if (this.tbConsoleCalibrate.InvokeRequired)
            {
                SetTextCallback d = new SetTextCallback(TextBoxWriteLine);
                this.Invoke(d, new object[] {data});
            }
            else
            {
                this.tbConsoleCalibrate.AppendText(currentDateTime + " | " + data);
                this.tbConsoleCalibrate.AppendText(Environment.NewLine);
            }

            
            //tbConsoleCalibrate.AppendText(currentDateTime + " | " + data);

            //https://stackoverflow.com/questions/10775367/cross-thread-operation-not-valid-control-textbox1-accessed-from-a-thread-othe

            //
        }
    }
}
