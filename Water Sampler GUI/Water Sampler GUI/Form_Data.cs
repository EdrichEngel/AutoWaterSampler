using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO.Ports;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;
using static System.Windows.Forms.VisualStyles.VisualStyleElement;

namespace Water_Sampler_GUI
{
    public partial class Form_Data : Form
    {
        private string _receivedData;
        private bool _received = false;
        private Form_Welcome _formWelcome;
        private string textFileName;
        public Form_Data(Form_Welcome formWelcome)
        {
            InitializeComponent();

            _formWelcome = formWelcome;
        }

        private void frmData_Load(object sender, EventArgs e)
        {

        }

        private void btnExit_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void btnScan_Click(object sender, EventArgs e)
        {
            textFileName = null;
            _formWelcome.SerialPortInstance.DiscardInBuffer();
            _formWelcome.SerialPortInstance.WriteLine("FR#");
            btnRead.Enabled = false;
            btnDownload.Enabled = false;

            _receivedData = null;
            _formWelcome.SerialPortInstance.DataReceived += SerialPort_DataReceived;

            bool success = SpinWait.SpinUntil(() => _receivedData != null, TimeSpan.FromSeconds(3));

            _formWelcome.SerialPortInstance.DataReceived -= SerialPort_DataReceived;

            if (success)
            {

                DecodeString();

            }
            else
            {
                cmbxFiles.Items.Clear();

                MessageBox.Show("No Resoponse From Device.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);

            }
        }
        private void SerialPort_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            SerialPort serialPort = (SerialPort)sender;
            _receivedData = serialPort.ReadLine();

            _received = true;



            //TextBoxWriteLine(_receivedData);

        }

        private void DecodeString()
        {
            int stringLength = _receivedData.Length;
            int nextPos = 0;
            string fileNameRead;
            //string tempOutput;



            string temp = _receivedData.Substring(3, (stringLength - 3));

            if (_receivedData[0] == 'F' && _receivedData[1] == 'W')
            {
                cmbxFiles.Items.Clear();
                while (temp.IndexOf('#') > 0)
                {
                    nextPos = temp.IndexOf('#');
                    fileNameRead = temp.Substring(0, nextPos);
                    cmbxFiles.Items.Add(fileNameRead);
                    temp = temp.Substring(nextPos + 1, (temp.Length - nextPos - 1));
                }



            }
        }

        private void btnRead_Click(object sender, EventArgs e)
        {
            _formWelcome.SerialPortInstance.DiscardInBuffer();
            tbTextFile.Clear();
            _formWelcome.SerialPortInstance.WriteLine("DR#" + textFileName + "#");
            btnRead.Enabled = false;
            btnDownload.Enabled = false;

            _receivedData = null;
            _formWelcome.SerialPortInstance.DataReceived += SerialPort_DataReceived;

            bool success = SpinWait.SpinUntil(() => _receivedData != null, TimeSpan.FromSeconds(3));

            _formWelcome.SerialPortInstance.DataReceived -= SerialPort_DataReceived;

            if (success)
            {
                DecodeStringTextFile();

            }
            else
            {
               

                MessageBox.Show("No Resoponse From Device.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);

            }


            // if successful
        }

        private void DecodeStringTextFile()
        {
            int stringLength = _receivedData.Length;
            int nextPos = 0;
            string textVar;
            //string tempOutput;



            string temp = _receivedData.Substring(3, (stringLength - 3));

            if (_receivedData[0] == 'D' && _receivedData[1] == 'W')
            {
                
                while (temp.IndexOf('#') > 0)
                {
                    nextPos = temp.IndexOf('#');
                    textVar = temp.Substring(0, nextPos);
                    TextBoxWriteLine(textVar);
                  
                    temp = temp.Substring(nextPos + 1, (temp.Length - nextPos - 1));
                }

                btnDownload.Enabled = true;

            }
        }


        private void TextBoxWriteLine(string data)
        {
            tbTextFile.AppendText(data);
            tbTextFile.AppendText(Environment.NewLine);
        }

        private void cmbxFiles_SelectedIndexChanged(object sender, EventArgs e)
        {
            if(cmbxFiles.SelectedIndex != -1)
            {
                btnRead.Enabled = true;
                textFileName = cmbxFiles.SelectedItem.ToString();
            }
           

        }

        private void btnDownload_Click(object sender, EventArgs e)
        {

            // Create an instance of SaveFileDialog
            SaveFileDialog saveFileDialog = new SaveFileDialog();

            // Set initial directory and filter for text files
            saveFileDialog.InitialDirectory = Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments);
            saveFileDialog.Filter = "Text Files (*.txt)|*.txt|All Files (*.*)|*.*";

            if (saveFileDialog.ShowDialog() == DialogResult.OK)
            {
                // Get the selected file path
                string filePath = saveFileDialog.FileName;

                // Get the text from the TextBox
                string textToSave = tbTextFile.Text;

                try
                {
                    // Write the text to the selected file
                    File.WriteAllText(filePath, textToSave);
                    MessageBox.Show("Text saved to file successfully.", "File Saved", MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
                catch (Exception ex)
                {
                    MessageBox.Show($"An error occurred: {ex.Message}", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
            }

        }
    }
}
