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
using static System.ActivationContext;

namespace Water_Sampler_GUI
{
    public partial class Form_Connect : Form
    {

        private string[] ports;
        private bool portSelected = false;


        public Form_Connect()
        {
            InitializeComponent();


            this.FormClosing += Form_Connect_FormClosing;


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
            if(cmbxPort.SelectedIndex != -1)
            {
                portSelected = true;
                TextBoxWriteLine("Com Port: " + cmbxPort.Items[cmbxPort.SelectedIndex] + " selected");
               
            }
            else
            {
                portSelected = false;
            }

            btnConnect.Enabled = portSelected;
            btnRefresh.Enabled = portSelected;
        }

        private void btnConnect_Click(object sender, EventArgs e)
        {
            if(portSelected == true)
            {
                TextBoxWriteLine("Connecting to: " + cmbxPort.Items[cmbxPort.SelectedIndex]); 
              
                Form_Welcome.bConnected = true;
                TextBoxWriteLine("Connection Successful");
               

            }
            else
            {
                MessageBox.Show("Please select a communication port.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void Form_Connect_Load(object sender, EventArgs e)
        {
            Form_Welcome.bConnected = false;
        }

        private void TextBoxWriteLine(string data)
        {
            string currentDateTime = DateTime.Now.ToString("HH:mm:ss");
            tbConsole.AppendText(currentDateTime + " | " + data);
            tbConsole.AppendText(Environment.NewLine);
        }
    }
}
