using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;
using static System.ActivationContext;

namespace Water_Sampler_GUI
{
    public partial class Form_Connect : Form
    {

        private string[] ports;
        private bool portSelected = false;
        private bool errorDetected = false;

        private string _receivedData;

        private Form_Welcome _formWelcome;

        public Form_Connect(Form_Welcome formWelcome)
        {
            InitializeComponent();


            this.FormClosing += Form_Connect_FormClosing;

            _formWelcome = formWelcome;


            if (_formWelcome.SerialPortInstance.IsOpen)
            {
                TextBoxWriteLine(_formWelcome.SerialPortInstance.PortName + " selected");
                portSelected = true;
                Form_Welcome.bConnected = true;
                btnScan.Enabled = false;
                btnConnect.Enabled = false;
                btnDisconnect.Enabled = true;
                btnRefresh.Enabled = true;

            }
            else
            {
                btnScan.Enabled = true;
                btnConnect.Enabled = false;
                btnDisconnect.Enabled = false;
                btnRefresh.Enabled = false;
            }

        }

        private void btnSave_Click(object sender, EventArgs e)
        {


            this.Close();
        }

        private void Form_Connect_FormClosing(Object sender, FormClosingEventArgs e)
        {

        }

        private void panel1_Paint(object sender, PaintEventArgs e)
        {

        }

        private void btnScan_Click(object sender, EventArgs e)
        {
           /* if (_formWelcome.SerialPortInstance.IsOpen)
            {
                _formWelcome.SerialPortInstance.Close();
            }*/

            cmbxPort.SelectedIndex = -1;
            TextBoxWriteLine("Scanning All Communication Ports");
            ports = SerialPort.GetPortNames();
            cmbxPort.Items.Clear();
            portSelected = false;
            foreach (string port in ports)
            {
                cmbxPort.Items.Add(port);
            }
            TextBoxWriteLine("Scanning Complete");


        }

        private void cmbxPort_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (cmbxPort.SelectedIndex != -1)
            {
                portSelected = true;
                TextBoxWriteLine(cmbxPort.Items[cmbxPort.SelectedIndex] + " selected");

            }
            else
            {
                portSelected = false;
            }

            btnConnect.Enabled = portSelected;
        }

        private void btnConnect_Click(object sender, EventArgs e)
        {
            if (portSelected == true)
            {
                errorDetected = false;
                TextBoxWriteLine("Connecting to: " + cmbxPort.Items[cmbxPort.SelectedIndex]);


                if (_formWelcome.SerialPortInstance.IsOpen)
                {
                    _formWelcome.SerialPortInstance.Close();
                }


                _formWelcome.SerialPortInstance.PortName = cmbxPort.Items[cmbxPort.SelectedIndex].ToString();
                _formWelcome.SerialPortInstance.BaudRate = 115200;

                try
                {
                    _formWelcome.SerialPortInstance.Open(); // Open the serial port
                    TextBoxWriteLine("Connection Established");
                }
                catch (Exception errorTemp)
                {
                    MessageBox.Show("Communication port cannot be opened. Error " + errorTemp.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    errorDetected = true;
                }

                if (!errorDetected)
                {

                    if (confirmDevice())
                    {
                        TextBoxWriteLine("Device Confirmed");
                        Form_Welcome.bConnected = true;
                        TextBoxWriteLine("Connection Successful");
                        btnConnect.Enabled = false;
                        btnScan.Enabled = false;
                        btnDisconnect.Enabled = true;
                        btnRefresh.Enabled = true;
                    }
                    else
                    {
                        Form_Welcome.bConnected = false;
                        btnRefresh.Enabled = false;
                        TextBoxWriteLine("Incompatible Device"); 
                        MessageBox.Show("Incompatible Device Selected.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                        _formWelcome.SerialPortInstance.Close();
                        TextBoxWriteLine("Connection Unsuccessful");
                    }
                }

            }
            else
            {
                MessageBox.Show("Please select a communication port.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void Form_Connect_Load(object sender, EventArgs e)
        {
            bool temp = false;
             int portIndex = -1;

            if (_formWelcome.SerialPortInstance.IsOpen)
            {

                Form_Welcome.bConnected = true;



                cmbxPort.SelectedIndex = -1;
                TextBoxWriteLine("Scanning All Communication Ports");
                ports = SerialPort.GetPortNames();
                cmbxPort.Items.Clear();
                
                foreach (string port in ports)
                {
                    cmbxPort.Items.Add(port);
                    

                    if ((port != _formWelcome.SerialPortInstance.PortName)&&(temp == false))
                    {
                        portIndex++;

                    }
                    else
                    {
                        if (temp == false)
                        {
                            portIndex++;
                        }
                        temp = true;
                    }

                    
                }
               

               

                cmbxPort.SelectedIndex = portIndex;

                btnConnect.Enabled = false;
                



            }
            else
            {

                Form_Welcome.bConnected = false;
            }
        }

        private void TextBoxWriteLine(string data)
        {
            string currentDateTime = DateTime.Now.ToString("HH:mm:ss");
            tbConsole.AppendText(currentDateTime + " | " + data);
            tbConsole.AppendText(Environment.NewLine);
        }

        private bool confirmDevice()
        {


            TextBoxWriteLine("Confirming Device");
            _formWelcome.SerialPortInstance.WriteLine("Penny is a freeloader.");


            _receivedData = null;
            _formWelcome.SerialPortInstance.DataReceived += SerialPort_DataReceived;

            // Wait for input for 3 seconds
            bool success = SpinWait.SpinUntil(() => _receivedData != null, TimeSpan.FromSeconds(3));

            _formWelcome.SerialPortInstance.DataReceived -= SerialPort_DataReceived;

            if (success)
            {
             
            }
            else
            {
                TextBoxWriteLine("No Repsonse from device");
                MessageBox.Show("No Resoponse From Device.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
             
                return false;
                
            }

            if (_receivedData.Substring(0,9) == "No Spaces")
            {
                return true;
            }
            else
            {
                TextBoxWriteLine("Error: " + _receivedData);
                return false;
            }

        }

        private void SerialPort_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            SerialPort serialPort = (SerialPort)sender;
            _receivedData = serialPort.ReadLine();
        }

        private void bntDisconnect_Click(object sender, EventArgs e)
        {
            if (_formWelcome.SerialPortInstance.IsOpen)
            {
                _formWelcome.SerialPortInstance.Close();
            }
            Form_Welcome.bConnected = false;
            btnConnect.Enabled = true;
            btnScan.Enabled = true;
            btnDisconnect.Enabled = false;
            btnRefresh.Enabled = false;
        }

        
    }
}
