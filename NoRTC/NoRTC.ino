
#include <ESP32Servo.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "FS.h"
#include "SD.h"
#include "SPI.h"
#include "cmath"
#include <Wire.h>           // Include the Wire library for I2C communication
#include "RTClib.h"         // Include the RTC library

#define pinSwitchMode 10 // Change.......................................................................................

#define onboardLedPin 2
#define temperaturePin 4
#define servoPin 13
#define relayPump 26
#define relayPower 14
#define turbidityPin 27
#define cardDetectPin 34

#define sToMs 1000
#define sToUs 1000000
#define msToUs 1000

// Define Variables
float servoPos = 0;  // Variable to store the servo position
float turbidityReading = 0;
float temperatureReading = 0;
float tempCoefA = 0;
float tempCoefB = 0;
float turbCoefA = 0;
float turbCoefB = 0;
float turbCoefC = 0;
float turbVal = 0;
float tempVal = 0;

bool sdCardDetected = 0;
bool sdCardDetectedPrev = 0;
bool delayFlag = 0;
bool fileCreatedOnStartup = 1;
bool failedToReadFile = 0;
bool serialDataReceivedState = false;

byte stateSwitchMode = 0;

int curDelay = 0;
int frequency = 1000;
int sampleCount = 0;
int duplicateFileCount = 0;

unsigned long Timer;

byte sdInit = 0;
byte state = 0;


String sdDataWrite;
String fileNameSettings = "/Settings.txt";
String fileNameData = "/Data.txt";
String fileNameCoef = "/Coefficients.txt";
String fileNameCoefTemp = "/CoefficientsTemp.txt";
String sampleLocation = "N/A";
String sampleDate = "N/A";
String sampleCountString = "0";
String serialDataReceived = "";
String serialDataProcessed = "";
String guiFileNameText;
String guiFrequencyText;
String guiLocationText;
String guiDateText;
String guiSampleCountText;

RTC_DATA_ATTR int bootCycleCount = 0;
RTC_DATA_ATTR int sampleCurrent = 1;

esp_sleep_wakeup_cause_t wakeup_reason;





// Define Constants
static const byte portPos1 = 0;
static const byte portPos2 = 26;
static const byte portPos3 = 51;
static const byte portPos4 = 77;
static const byte portPos5 = 103;
static const byte portPos6 = 129;
static const byte portPos7 = 154;
static const byte portPos8 = 180;

// Define Functions
int servoDegrees(byte portNumber);

bool testFileExistance(fs::FS &fs, String path);

float getTurbidity();
float getTemperature();

void initSDCard();
void setupSDCard();
void appendFile(fs::FS &fs, String path, String message);
void readSettingsFile(fs::FS &fs, String path);
void guiReadCoefFile(fs::FS &fs, String path);
void decodeSDString(String dataString);
void splitStrings(String data);
void writeGeneralInfoToSD();
void duplicateFileDetected();
void print_wakeup_reason();
void startDeepSleep();
void deepSleepSetup(int Time);
void IRAM_ATTR sdCardRemove();
void sdCardCheckDuringLoop();
void listDir(fs::FS &fs, const char * dirname, uint8_t levels);
void deleteFile(fs::FS &fs, String path);
void renameFile(fs::FS &fs, String path1, String path2);

void monitorMode();
void collectionMode();
void readSerial();
void guiReadCoefficients();
void guiNewCoefficients();
void guiReadSensorValue();
void guiMonitor();
void guiFileName();
void guiFileDataRead();
void guiConfig();
void guiNewConfig();
void decodeConfigFile(String data);
void decodeCoefFile(String tempData, byte selectedSensor);
void guiReadConfigFile(fs::FS &fs, String path);
void DecodeNewConfig(String tempData);
void createNewConfigFile();

String readDataFile(fs::FS &fs, String path);

// Define Objects
Servo valveServo;                               // Create servo object
OneWire oneWire(temperaturePin);                // Create a OneWire object for the temperature sensor
DallasTemperature temperatureSensor(&oneWire);  // Create a DallasTemperature object by using the DallasTemperature library
//RTC_DS3231 rtc;                                 // Create an RTC object




void setup() {


 pinMode(relayPump, OUTPUT);
 pinMode(relayPower, OUTPUT);

  digitalWrite(relayPower, HIGH);

  // Start Serial Communication
  Serial.begin(115200);

 Serial.println("Relay on");
  delay(2000);
  

  Wire.begin();             // Start the I2C communication


 /* if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
  if (rtc.lostPower()) {
   // Serial.println("RTC lost power, lets set the time!");
    
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }*/

  //Print boot cycle number and the wakeup reason
 // Serial.println("Boot number: " + String(bootCycleCount));
  print_wakeup_reason();

  pinMode(pinSwitchMode, INPUT);

  // Onboard LED setup
  pinMode(onboardLedPin, OUTPUT);

  // SD Card Breakout Board Setup
  pinMode(cardDetectPin, INPUT);
  //attachInterrupt(cardDetectPin, sdCardRemove, FALLING);
  // attachInterrupt(cardDetectPin, sdCardInsert, RISING);
  attachInterrupt(cardDetectPin, sdCardRemove, CHANGE);

  // Allow allocation of timers
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);

  // Setup of servo
  valveServo.setPeriodHertz(50);
  valveServo.attach(servoPin, 500, 2500);
  // Moving servo to default position
  valveServo.write(servoDegrees(0));

  // Starts up the temperature sensor's OneWire communication protocol
  temperatureSensor.begin();

  setupSDCard();

 


  if (bootCycleCount == 0) {
    while (testFileExistance(SD, fileNameData)) {
      duplicateFileDetected();
    }

    if (!testFileExistance(SD, fileNameData)) {
      //Serial.println("Writing general info");
      writeGeneralInfoToSD();
      state = 1;
    } else {
      //Serial.println("Error while creating duplicate file.");
      state = 0;
    }
  }
  bootCycleCount++;
  deepSleepSetup(frequency);

  ReadCoefFile(SD,fileNameCoef);
  /*Serial.println(tempCoefA);
  Serial.println(tempCoefB);
  Serial.println(turbCoefA);
  Serial.println(turbCoefB);
  Serial.println(turbCoefC);*/
}








void loop() {
  // put your main code here, to run repeatedly:
 
    // Update stateSwitchMode here
    //stateSwitchMode = digitalRead(pinSwitchMode);
    stateSwitchMode = 0;

    if (stateSwitchMode == 0){
      // Monitor Mode
      monitorMode(); 
    } else {
      // Collection Mode
      collectionMode();
    }


}










int servoDegrees(byte portNumber) {

  switch (portNumber) {
    case 1:
      return portPos1;
      break;
    case 2:
      return portPos2;
      break;
    case 3:
      return portPos3;
      break;
    case 4:
      return portPos4;
      break;
    case 5:
      return portPos5;
      break;
    case 6:
      return portPos6;
      break;
    case 7:
      return portPos7;
      break;
    case 8:
      return portPos8;
      break;
    default:
      return 0;
      break;
  }
}


// This function determines the turbidity in NTUs from the raw ADC value
float getTurbidity() {
  float voltage;
  float turb;
  float rawADC = analogRead(turbidityPin);

  //Serial.print("raw: ");
  //Serial.println(rawADC);
  // Convert the raw ADC value to the output voltage from the sensor
  voltage = (float)(rawADC * 5 / 4095.0 * 0.8776 + 0.412);
  /* Serial.print("VOLTAGE: ");
  Serial.println(voltage);*/
  // Convert the output voltage from the sensor to turbidity in NTUs
  turb = (voltage);
  return turb;
}

float getTemperature() {
  temperatureSensor.requestTemperatures();
  return temperatureSensor.getTempCByIndex(0);
}


void initSDCard() {
  uint8_t sdCardType = SD.cardType();
  bool sdError = 0;

  if (!SD.begin(32)) {
    //Serial.println("SD Card Mount Failed");
    sdError = 1;
    return;
  }
  //Serial.println("SD Card Mounted");
  if (sdCardType == CARD_NONE) {
   // Serial.println("No SD card attached");
    sdError = 1;
    return;
  } else {
    sdInit = 1;
   // Serial.println("SD Card Attached");
  }
}


// Using function written for SD card example
void appendFile(fs::FS &fs, String path, String message) {
  //Serial.printf("Appending to file: %s\n", path.c_str());

  File file = fs.open(path.c_str(), FILE_APPEND);
  if (!file) {
   // Serial.println("Failed to open file for appending");
    return;
  }
  if (file.print(message.c_str())) {
    //Serial.println("Message appended");
  } else {
   // Serial.println("Append failed");
  }
  file.close();
}


bool testFileExistance(fs::FS &fs, String path) {

  //Serial.printf("Testing Existance Of: %s\n", path.c_str());

  File file = fs.open(path.c_str());
  if (!file) {
   // Serial.println("File Does Not Exists");
    return 0;
  } else {
    //Serial.println("File Exists");
    return 1;
  }
}


void readSettingsFile(fs::FS &fs, String path) {
  String sdDataRead;

  //initSDCard();

  //Serial.printf("Reading Settings From: %s\n", path.c_str());

  File file = fs.open(path.c_str());
  if (!file) {
  //  Serial.println("Failed to open file for reading");
    failedToReadFile = 1;
    return;
  }
  failedToReadFile = 0;

  while (file.available()) {
    sdDataRead = file.readString();
    splitStrings(sdDataRead);
    //Serial.print(sdDataRead);
  }
  file.close();

  if (!fileCreatedOnStartup) {
    while (testFileExistance(SD, fileNameData)) {
      duplicateFileDetected();
    }
  }

  if (!testFileExistance(SD, fileNameData)) {
    //Serial.println("Writing general info");
    writeGeneralInfoToSD();
  }
}


void splitStrings(String data) {
  byte newLineReady = 1;
  int newLineCharPos = 1;
  int prevLineCharPos = 0;

  while (newLineReady) {
    newLineCharPos = data.indexOf('\n', newLineCharPos + 1);
    /* Serial.print("Char found at: ");
    Serial.println(newLineCharPos);*/
    if (newLineCharPos == -1) {
      newLineReady = 0;
      newLineCharPos = data.length();
    }


    decodeSDString(data.substring(prevLineCharPos, newLineCharPos));
    /* Serial.print("Start Line: ");
    Serial.println(data.substring(prevLineCharPos, newLineCharPos));
    Serial.print("End Line: ");*/
    prevLineCharPos = newLineCharPos + 1;
  }
}

void decodeSDString(String dataString) {
  int unitFactor, temp, newLineCharPos;
  String unitString, durationString;

  if (strcmp(dataString.substring(0, 8).c_str(), "FileName") == 0) {
    // File Name
    fileNameData = dataString.substring(9, dataString.length());
  //  Serial.print("File Name Changed To: ");
    fileNameData = "/" + fileNameData;
   // Serial.println(fileNameData);

  } else {
    if (strcmp(dataString.substring(0, 9).c_str(), "Frequency") == 0) {
      // Frequency
      durationString = dataString.substring(10, dataString.length() - 1);
      unitString = dataString.substring(dataString.length() - 2, dataString.length() - 1);



      temp = durationString.toInt();

      if ((strcmp(unitString.c_str(), "s") == 0) || (strcmp(unitString.c_str(), "S") == 0)) {
        unitFactor = 1 * sToMs;
      } else {
        if ((strcmp(unitString.c_str(), "m") == 0) || (strcmp(unitString.c_str(), "M") == 0)) {
          unitFactor = 60 * sToMs;
        } else {
          if ((strcmp(unitString.c_str(), "h") == 0) || (strcmp(unitString.c_str(), "H") == 0)) {
            unitFactor = 3600 * sToMs;
          }
        }
      }



      frequency = temp * unitFactor;

     // Serial.print("Frequency Changed To: ");
     // Serial.println(frequency);
    } else {
      if (strcmp(dataString.substring(0, 8).c_str(), "Location") == 0) {
        sampleLocation = dataString.substring(9, dataString.length() - 1);
       // Serial.print("Sample Location: ");
       // Serial.println(sampleLocation);
      } else {
        if (strcmp(dataString.substring(0, 4).c_str(), "Date") == 0) {
          sampleDate = dataString.substring(5, dataString.length() - 1);
          //Serial.print("Sample Date: ");
          //Serial.println(sampleDate);
        } else {
          if (strcmp(dataString.substring(0, 11).c_str(), "SampleCount") == 0) {
            sampleCountString = dataString.substring(12, dataString.length());
            sampleCount = sampleCountString.toInt();
           // Serial.print("Sample Count: ");
           // Serial.println(sampleCount);
          } else {
          //  Serial.println("Error occured while decoding the settings file.");
          }
        }
      }
    }
  }
}



void writeGeneralInfoToSD() {

  sdDataWrite = "Location,";
  sdDataWrite += sampleLocation;
  sdDataWrite += ",";
  sdDataWrite += "\n";
  appendFile(SD, fileNameData, sdDataWrite);
  sdDataWrite = "Date,";
  sdDataWrite += sampleDate;
  sdDataWrite += ",";
  sdDataWrite += "\n";
  appendFile(SD, fileNameData, sdDataWrite);
  sdDataWrite = "Frequency,";
  sdDataWrite += frequency;
  sdDataWrite += ",";
  sdDataWrite += "\n";
  appendFile(SD, fileNameData, sdDataWrite);
  sdDataWrite = "Sample Count,";

  if (sampleCount == 0) {
    sampleCountString = "0";
  }
  sdDataWrite += sampleCountString;
  sdDataWrite += ",";
  sdDataWrite += "\n";
  appendFile(SD, fileNameData, sdDataWrite);
  sdDataWrite = "SampleNumber,Turbidity,Temperature\n";
  appendFile(SD, fileNameData, sdDataWrite);
}

void duplicateFileDetected() {
  byte extraChar = 0;
  float temp = 0;

  if (duplicateFileCount == 0) {
    duplicateFileCount++;
    fileNameData = fileNameData.substring(0, fileNameData.length() - 5);
    fileNameData += "(";
    fileNameData += duplicateFileCount;
    fileNameData += ").txt";
  } else {


    temp = log10(duplicateFileCount);

    extraChar = ceil(temp);

    /* Serial.print("Dup file = ");
    Serial.println(duplicateFileCount);

    Serial.print("temp = ");
    Serial.println(temp);
    Serial.print("extra char = ");
    Serial.println(extraChar);
    Serial.print("pow = ");
    Serial.println(pow(10,extraChar));*/

    if (pow(10, extraChar) == duplicateFileCount) {
      extraChar++;
      // Serial.println("Special case");
    }


    fileNameData = fileNameData.substring(0, fileNameData.length() - 5 - extraChar);
    duplicateFileCount++;
    fileNameData += duplicateFileCount;
    fileNameData += ").txt";
  }
/*  Serial.print("Duplicate file found: ");
  Serial.println(duplicateFileCount);
  Serial.println(fileNameData);
  Serial.println("");*/
}

void print_wakeup_reason() {
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();

  switch (wakeup_reason) {
  /*  case ESP_SLEEP_WAKEUP_EXT0: Serial.println("Wakeup caused by external signal using RTC_IO"); break;
    case ESP_SLEEP_WAKEUP_EXT1: Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
    case ESP_SLEEP_WAKEUP_TIMER: Serial.println("Wakeup caused by timer"); break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD: Serial.println("Wakeup caused by touchpad"); break;
    case ESP_SLEEP_WAKEUP_ULP: Serial.println("Wakeup caused by ULP program"); break;
    default: Serial.printf("Wakeup was not caused by deep sleep: %d\n", wakeup_reason); break;*/
  }
}


void startDeepSleep() {
 // Serial.println("Going to sleep now");
  delay(1000);
  Serial.flush();
  esp_deep_sleep_start();
  //Serial.println("This will never be printed");
}

void deepSleepSetup(int Time) {
  if (Time != -1) {
    esp_sleep_enable_timer_wakeup(Time * msToUs);
   // Serial.println("Setup ESP32 to sleep for every " + String(frequency / 1000) + " Seconds");
  } else {
    esp_sleep_disable_wakeup_source(ESP_SLEEP_WAKEUP_TIMER);
  }
}

void IRAM_ATTR sdCardRemove() {

  if (digitalRead(cardDetectPin) == 0) {
    delayFlag = 0;
    sampleCount = 0;
    duplicateFileCount = 0;
    fileCreatedOnStartup = 0;
    state = 0;
  } else {
    delayFlag = 0;
    duplicateFileCount = 0;
    fileCreatedOnStartup = 0;
  }
}

void setupSDCard(){
   sdCardDetected = digitalRead(cardDetectPin);
  sdCardDetectedPrev = sdCardDetected;

  if (sdCardDetected) {
    initSDCard();
    if (!sdInit) {
      delay(50);
      initSDCard();
    }
  }

  if (sdInit) {
    readSettingsFile(SD, fileNameSettings);
  }
}

void sdCardCheckDuringLoop(){
  
    sdCardDetected = digitalRead(cardDetectPin);
    digitalWrite(onboardLedPin, sdCardDetected);

    if ((sdCardDetected != sdCardDetectedPrev) || (failedToReadFile)) {
      if (sdCardDetected) {
      //  Serial.println("SD Card Detected");
        if (sdCardDetected) {
          initSDCard();
          if (!sdInit) {
            delay(50);
            initSDCard();
          }
        }

        if (sdInit) {
          readSettingsFile(SD, fileNameSettings);
          sampleCurrent = 1;
        }
      } else {
        //Serial.println("SD Card Removed");
        frequency = 1000;
        sdInit = 0;
        sampleCurrent = 1;
        sampleCount = 0;
      }
    }
    sdCardDetectedPrev = sdCardDetected;
}

void collectionMode(){
 if (sampleCount == 0) {
    state = 0;
  } else {
    state = 1;
  }
  if ((sampleCurrent <= sampleCount) || (sampleCount == 0)) {


    //if (delayFlag == 0) {

    sdCardCheckDuringLoop();

    turbidityReading = getTurbidity();
    temperatureReading = getTemperature();
    /*Serial.print("Sample ");
    Serial.print(sampleCurrent);
    Serial.print(" of ");
    Serial.println(sampleCount);
    Serial.print("Turbidity: ");
    Serial.println(turbidityReading);
    Serial.print("Temperature: ");
    Serial.println(temperatureReading);
    Serial.println(" ");*/

    if (sdInit) {
      sdDataWrite = sampleCurrent;
      sdDataWrite += ",";
      sdDataWrite += turbidityReading;
      sdDataWrite += ",";
      sdDataWrite += temperatureReading;
      sdDataWrite += "\n";
      appendFile(SD, fileNameData, sdDataWrite.c_str());
    } else if (!sdInit && sdCardDetected) {
      initSDCard();
    }

    sampleCurrent++;
    delayFlag = 1;
    Timer = millis();

    if (sampleCurrent > sampleCount) {
      if (sampleCount != -1) {
      //  Serial.println("System has finished sampling");
      }
      sampleCount = -1;
      deepSleepSetup(-1);
    }

    startDeepSleep();

    /*} else if (delayFlag == 1) {

      if (millis() > (Timer + frequency)) {
        // if the time has passed
        delayFlag = 0;
        curDelay = 0;
      }
    }*/
  } else {
    if (sampleCount != -1) {
     // Serial.println("System has finished sampling");
    }
    sampleCount = -1;
    deepSleepSetup(-1);
    startDeepSleep();

    //Serial.println("Stuck");
  }
}

void monitorMode(){

  readSerial();

  if(serialDataReceivedState == true){

    if (serialDataReceived[1] == '#'){
      //2.4;2.5;2.6
      switch(serialDataReceived[0]){
        case 'R':
        //2.4
          guiReadCoefficients();
        break;
        case 'C':
        //2.5  
          guiNewCoefficients();
        break;
        case 'S': 
        //2.6  
          guiReadSensorValue();
        break;
        default:

        break;
      }



    } else if(serialDataReceived[2] == '#'){
      //2.7;2.8;2.9;2.10;2.11

      if(serialDataReceived.substring(0,2) == "MR"){
        //2.7
        guiMonitor();

      } else if(serialDataReceived.substring(0,2) == "FR"){
        //2.8
        guiFileName();
        
      } else if (serialDataReceived.substring(0,2) == "DR"){
        //2.9
        guiFileDataRead();

      } else if(serialDataReceived.substring(0,2) == "CR"){
        //2.10
        guiConfig();

      } else if(serialDataReceived.substring(0,2) == "CS"){
        //2.11
        guiNewConfig();

      }


    
    }else if(serialDataReceived.substring(0,22) == "Penny is a freeloader."){
      //2.3
      Serial.println("No Spaces");    
    }

    serialDataReceivedState = false; 
  }
}


void readSerial(){
  char tempSerialData;
  
  serialDataReceived = "";
  
  while (Serial.available() > 0)
  {
    serialDataReceivedState = true;
    tempSerialData = Serial.read(); //reads serial input
    serialDataReceived += tempSerialData;
  }

}


void guiReadCoefficients(){
  
  guiReadCoefFile(SD,fileNameCoef);


}

void guiNewCoefficients(){

  guiWriteCoefFile(SD,fileNameCoef);

  deleteFile(SD, fileNameCoef);
  renameFile(SD, fileNameCoefTemp, fileNameCoef);
}

void guiReadSensorValue(){

  String transmission = "SR#";



  if (serialDataReceived.substring(2,3) == "0"){
      //Temperature
      transmission += getTempRawString();
      transmission += "#";
      transmission += getTempProcessed();
      transmission += "#";



  } else if ((serialDataReceived.substring(2,3) == "1")){
    //Turbidity
      transmission += getTurbRawString();
      transmission += "#";
      transmission += getTurbProcessed();
      transmission += "#";
  }

  Serial.println(transmission);


}

void guiMonitor(){
  String temp = "";

  temp = getTempRawString();
  temp = getTurbRawString();

  String transmissionString = "MW#";
  transmissionString += getTime();
  transmissionString += "#";
  transmissionString += getBattery();
  transmissionString += "#";
  transmissionString += getSDMonitorValues();
  transmissionString += "#";
  transmissionString += getTempProcessed();
  
 
  transmissionString += "#";
  transmissionString += getTurbProcessed();
 
  transmissionString += "#";

  Serial.println(transmissionString);




}

void guiFileName(){

  listDir(SD, "/", 1);


}

void guiFileDataRead(){
  String fileNameToRead = "/" + serialDataReceived.substring(3,serialDataReceived.length()-2);
  String transmissionString = "DW#";






  transmissionString += readDataFile(SD,fileNameToRead);
  //transmissionString += "#";

  Serial.println(transmissionString);
}

void guiConfig(){
  String tempTransmission;
  guiReadConfigFile(SD,fileNameSettings);

  tempTransmission = "CW#";
  tempTransmission += guiFileNameText;
  tempTransmission += "#";
  tempTransmission += guiDateText;
  tempTransmission += "#";
  tempTransmission += guiLocationText;
  tempTransmission += "#";
  tempTransmission += guiSampleCountText;
  tempTransmission += "#";
  tempTransmission += guiFrequencyText;
  tempTransmission += "#";

  Serial.println(tempTransmission);

}

void guiNewConfig(){
  String tmpData;
  int nextPos;



  tmpData = serialDataReceived.substring(3,serialDataReceived.length());

  DecodeNewConfig(tmpData);
  deleteFile(SD,fileNameSettings);
  createNewConfigFile();
  
  
  



}

void listDir(fs::FS &fs, const char * dirname, uint8_t levels){
 
  Serial.print("FW#");
  File root = fs.open(dirname);
  if(!root){
    return;
  }
  if(!root.isDirectory()){
    return;
  }

  File file = root.openNextFile();
  while(file){
    if(file.isDirectory()){
      if(levels){
        listDir(fs, file.name(), levels -1);
      }
    } else {
      Serial.print(file.name());
      Serial.print("#");
    }
    file = root.openNextFile();
  }
  Serial.println("");
}

String readDataFile(fs::FS &fs, String path) {
  String sdDataRead = "";
  String incommingData;

  //initSDCard();

  //Serial.printf("Reading Settings From: %s\n", path.c_str());

  File file = fs.open(path.c_str());
  if (!file) {
    //Serial.println("Failed to open file for reading");
    failedToReadFile = 1;
    return "F*CK";
  }
  failedToReadFile = 0;

  while (file.available()) {
    incommingData = file.readStringUntil('\n');
    
   // if (incommingData.indexOf('\n') > 0){
            sdDataRead += incommingData;
  /*  } else{

      sdDataRead += incommingData.substring(0,incommingData.length()-1);
      sdDataRead += incommingData.indexOf('\n');
    }*/
    sdDataRead += "#";
    //Serial.print(sdDataRead);
  }
  file.close();
 
  return sdDataRead;
}

void guiReadConfigFile(fs::FS &fs, String path) {

  String incommingData;

  //initSDCard();

  //Serial.printf("Reading Settings From: %s\n", path.c_str());

  File file = fs.open(path.c_str());
  if (!file) {
    //Serial.println("Failed to open file for reading");
    failedToReadFile = 1;
    
  }
  failedToReadFile = 0;

  while (file.available()) {
    incommingData = file.readStringUntil('\n');

    decodeConfigFile(incommingData);
    
    
  }
  file.close();
 
  



}


void decodeConfigFile(String data){
  if(data.substring(0,8) == "FileName"){
    guiFileNameText = data.substring(9,data.length());
    if(guiFileNameText[guiFileNameText.length()]=='\n'){
      guiFileNameText = guiFileNameText.substring(0,guiFileNameText.length()-1);
    }
  } else if(data.substring(0,9) == "Frequency"){
    guiFrequencyText = data.substring(10,data.length());
    if(guiFrequencyText[guiFrequencyText.length()]=='\n'){
      guiFrequencyText = guiFrequencyText.substring(0,guiFrequencyText.length()-1);
    }
  } else if(data.substring(0,8) == "Location"){
    guiLocationText = data.substring(9,data.length());
    if(guiLocationText[guiLocationText.length()]=='\n'){
      guiLocationText = guiLocationText.substring(0,guiLocationText.length()-1);
    }
  } else if(data.substring(0,4) == "Date"){
    guiDateText = data.substring(5,data.length());
    if(guiDateText[guiDateText.length()]=='\n'){
      guiDateText = guiDateText.substring(0,guiDateText.length()-1);
    }
  } else if(data.substring(0,11) == "SampleCount"){
    guiSampleCountText = data.substring(12,data.length());
    if(guiSampleCountText[guiSampleCountText.length()]=='\n'){
      guiSampleCountText = guiSampleCountText.substring(0,guiSampleCountText.length()-1);
    }
  }
}

void deleteFile(fs::FS &fs, String path){
  //Serial.printf("Deleting file: %s\n", path);
  if(fs.remove(path.c_str())){
    //Serial.println("File deleted");
  } else {
   // Serial.println("Delete failed");
  }
}


void DecodeNewConfig(String tempData){
  int nextPos;
 
  
  //Serial.println(tempData);

  nextPos = tempData.indexOf('#');
  guiFileNameText = tempData.substring(0, nextPos);
  tempData = tempData.substring(nextPos + 1, (tempData.length()));
 // Serial.println(tempData);
  
  nextPos = tempData.indexOf('#');
  guiDateText = tempData.substring(0, nextPos);
  tempData = tempData.substring(nextPos + 1, (tempData.length()));
  //Serial.println(tempData);

  nextPos = tempData.indexOf('#');
  guiLocationText = tempData.substring(0, nextPos);
  tempData = tempData.substring(nextPos + 1, (tempData.length()));
 // Serial.println(tempData);

  nextPos = tempData.indexOf('#');
  guiSampleCountText = tempData.substring(0, nextPos);
  tempData = tempData.substring(nextPos + 1, (tempData.length()));
  //Serial.println(tempData);

  nextPos = tempData.indexOf('#');
  guiFrequencyText = tempData.substring(0, tempData.length()-2);
  //Serial.println(tempData);
}

void createNewConfigFile() {
  
  sdDataWrite = "FileName ";
  sdDataWrite += guiFileNameText;
  sdDataWrite += "\n";
  appendFile(SD, fileNameSettings, sdDataWrite);
  sdDataWrite = "Frequency ";
  sdDataWrite += guiFrequencyText;
  sdDataWrite += "\n";
  appendFile(SD, fileNameSettings, sdDataWrite);
  sdDataWrite = "Location ";
  sdDataWrite += guiLocationText;
  sdDataWrite += "\n";
  appendFile(SD, fileNameSettings, sdDataWrite);
  sdDataWrite = "Date ";
  sdDataWrite += guiDateText;
  sdDataWrite += "\n";
  appendFile(SD, fileNameSettings, sdDataWrite);
  sdDataWrite = "SampleCount ";
  sdDataWrite += guiSampleCountText;
  appendFile(SD, fileNameSettings, sdDataWrite);
}

void guiReadCoefFile(fs::FS &fs, String path) {

  String incommingData;

  //initSDCard();

  //Serial.printf("Reading Settings From: %s\n", path.c_str());

  File file = fs.open(path.c_str());
  if (!file) {
    //Serial.println("Failed to open file for reading");
    failedToReadFile = 1;
    
  }
  failedToReadFile = 0;

  while (file.available()) {
    incommingData = file.readStringUntil('\n');

    decodeCoefFile(incommingData, serialDataReceived.substring(2,serialDataReceived.length()).toInt());
    
    
  }
  file.close();

}

void ReadCoefFile(fs::FS &fs, String path) {

  String incommingData;

  //initSDCard();

  //Serial.printf("Reading Settings From: %s\n", path.c_str());

  File file = fs.open(path.c_str());
  if (!file) {
    //Serial.println("Failed to open file for reading");
    failedToReadFile = 1;
    
  }
  failedToReadFile = 0;

  while (file.available()) {
    incommingData = file.readStringUntil('\n');

    readCoefFromFile(incommingData);
    
    
  }
  file.close();

}

void guiWriteCoefFile(fs::FS &fs, String path) {

  String incommingData;

  //initSDCard();

  //Serial.printf("Reading Settings From: %s\n", path.c_str());

  File file = fs.open(path.c_str());
  if (!file) {
    //Serial.println("Failed to open file for reading");
    failedToReadFile = 1;
    
  }
  failedToReadFile = 0;

  serialDataReceived = serialDataReceived.substring(serialDataReceived.indexOf('#')+1,serialDataReceived.length());
  while (file.available()) {
    incommingData = file.readStringUntil('\n');

    
    decodePrevCoefFile(incommingData, serialDataReceived.substring(0,serialDataReceived.indexOf('#')).toInt());
    //appendFile(SD, "/log.txt", serialDataReceived.substring(0,serialDataReceived.indexOf('#')) + '\n');
    
  }
  file.close();

}

void decodeCoefFile(String tempData, byte selectedSensor){
  int nextPos;
  String Sensor;  
  String transmission = "RR#";


  //Serial.println(tempData);

  nextPos = tempData.indexOf('#');
  Sensor = tempData.substring(0, nextPos);
  tempData = tempData.substring(nextPos + 1, (tempData.length()));

  if(((Sensor == "Temperature")&&(selectedSensor == 0))||((Sensor == "Turbidity")&&(selectedSensor == 1))){
     transmission += tempData.substring(0, tempData.length());
     Serial.println(transmission);

    if(selectedSensor == 0){

        nextPos = tempData.indexOf('#');
        tempCoefA = tempData.substring(0, nextPos).toFloat();
        tempData = tempData.substring(nextPos + 1, (tempData.length()));
      
        nextPos = tempData.indexOf('#');
        tempCoefB = tempData.substring(0, nextPos).toFloat();
       /* Serial.println(tempCoefA);
        Serial.println(tempCoefB);*/

    } else if (selectedSensor == 1){
        nextPos = tempData.indexOf('#');
        turbCoefA = tempData.substring(0, nextPos).toFloat();
        tempData = tempData.substring(nextPos + 1, (tempData.length()));
      
        nextPos = tempData.indexOf('#');
        turbCoefB = tempData.substring(0, nextPos).toFloat();
        tempData = tempData.substring(nextPos + 1, (tempData.length()));
      
        nextPos = tempData.indexOf('#');
        turbCoefC = tempData.substring(0, nextPos).toFloat();
        /*Serial.println(turbCoefA);
        Serial.println(turbCoefB);
        Serial.println(turbCoefC);*/
    }

  } 
  
}

void readCoefFromFile(String tempData){
  int nextPos;
  String Sensor;  



  //Serial.println(tempData);

  nextPos = tempData.indexOf('#');
  Sensor = tempData.substring(0, nextPos);
  tempData = tempData.substring(nextPos + 1, (tempData.length()));


    if(Sensor == "Temperature"){

        nextPos = tempData.indexOf('#');
        tempCoefA = tempData.substring(0, nextPos).toFloat();
        tempData = tempData.substring(nextPos + 1, (tempData.length()));
      
        nextPos = tempData.indexOf('#');
        tempCoefB = tempData.substring(0, nextPos).toFloat();

    } else if (Sensor == "Turbidity"){
        nextPos = tempData.indexOf('#');
        turbCoefA = tempData.substring(0, nextPos).toFloat();
        tempData = tempData.substring(nextPos + 1, (tempData.length()));
      
        nextPos = tempData.indexOf('#');
        turbCoefB = tempData.substring(0, nextPos).toFloat();
        tempData = tempData.substring(nextPos + 1, (tempData.length()));
      
        nextPos = tempData.indexOf('#');
        turbCoefC = tempData.substring(0, nextPos).toFloat();

    }

  
  
}

void decodePrevCoefFile(String tempData, byte selectedSensor){
  int nextPos;
  String Sensor;  
  String newLine = "";
  String tempDataLine = "";


  //Serial.println(tempData);

  nextPos = tempData.indexOf('#');
  Sensor = tempData.substring(0, nextPos);
  tempData = tempData.substring(nextPos + 1, (tempData.length()));


  if(Sensor == "Temperature"){
    newLine = "Temperature#";

    if(selectedSensor == 0){
      //change coef
      tempDataLine = serialDataReceived.substring(2,serialDataReceived.length());
      newLine += tempDataLine;
    } else{
      tempDataLine = tempData;
      newLine += tempDataLine;
    }

        nextPos = tempDataLine.indexOf('#');
        tempCoefA = tempDataLine.substring(0, nextPos).toFloat();
        tempDataLine = tempDataLine.substring(nextPos + 1, (tempDataLine.length()));
      
        nextPos = tempDataLine.indexOf('#');
        tempCoefB = tempDataLine.substring(0, nextPos).toFloat();
       /* Serial.println(tempCoefA);
        Serial.println(tempCoefB);*/



  } else  if(Sensor == "Turbidity"){
    newLine = "Turbidity#";
    if(selectedSensor == 1){
      //change coef
      tempDataLine = serialDataReceived.substring(2,serialDataReceived.length());
      newLine += tempDataLine;
    } else{
      tempDataLine = tempData;
      newLine += tempDataLine;
    }


    
 
        nextPos = tempDataLine.indexOf('#');
        turbCoefA = tempDataLine.substring(0, nextPos).toFloat();
        tempDataLine = tempDataLine.substring(nextPos + 1, (tempDataLine.length()));
      
        nextPos = tempDataLine.indexOf('#');
        turbCoefB = tempDataLine.substring(0, nextPos).toFloat();
        tempDataLine = tempDataLine.substring(nextPos + 1, (tempDataLine.length()));
      
        nextPos = tempDataLine.indexOf('#');
        turbCoefC = tempDataLine.substring(0, nextPos).toFloat();
      /*  Serial.println(turbCoefA);
        Serial.println(turbCoefB);
        Serial.println(turbCoefC);*/
    

  }
  newLine += '\n';
  appendFile(SD, fileNameCoefTemp, newLine);

  
}

void renameFile(fs::FS &fs, String path1, String path2){
  //Serial.printf("Renaming file %s to %s\n", path1.c_str(), path2.c_str());
  if (fs.rename(path1, path2)) {
   // Serial.println("File renamed");
  } else {
  //  Serial.println("Rename failed");
  }
}

String getTime(){
  String temp = "";
  /*DateTime now = rtc.now();
  

  temp += now.year(); 
  temp += '/';
  temp += now.month();
  temp += '/';
  temp += now.day();
  temp += " ";
  temp += now.hour() ;
  temp += ':';
  temp += now.minute();
  temp += ':';
  temp += now.second();
*/



  temp += "2023"; 
  temp += '/';
  temp += "09";
  temp += '/';
  temp += "25";
  temp += " ";
  temp +=  "21";
  temp += ':';
  temp += "45";
  temp += ':';
  temp += "10";

  return temp;

}

String getBattery(){
  return "N/A";
}

String getSDMonitorValues(){
  String temp = "";
  bool continueSD = true;
  if(SD.begin(32)){
    temp += "Mounted#";    
  } else{
    temp += "N/A#";
    continueSD = false;
  }

  if (continueSD){
    uint64_t cardSize = SD.cardSize() / (1024 * 1024);
    temp += cardSize;
    temp += "MB#";
    uint64_t usedbytes = SD.usedBytes() / (1024 * 1024);
    temp += usedbytes;
    temp += "MB#";
   
    uint64_t div = round((usedbytes*100)/cardSize);
    
  
    temp += div;
    temp += "%";


  } else{
    temp += "N/A#";
    temp += "N/A#";
    temp += "N/A";
  }

return temp;

}

String getTurbProcessed(){

  float turb;
  float processedTurb;
  char turbChar[100];
  String turbString;

  turb = turbVal;

  processedTurb = turb*turb*turbCoefA + turbCoefB*turb + turbCoefC;
  //processedTurb = turb;
  dtostrf(processedTurb, 4, 2, turbChar);
  turbString = turbChar;

  return turbString;


}


String getTempProcessed(){

  float temp;
  float processedTemp;
  char tempChar[100];
  String tempString;

  temp = tempVal;

  processedTemp = temp*tempCoefA + tempCoefB;
  dtostrf(processedTemp, 4, 2, tempChar);
  tempString = tempChar;

  return tempString;
}

String getTempRawString(){

  float temp;
  float processedTemp;
  char tempChar[100];
  String tempString;

  temp = getTemperature();
  tempVal = temp;

  processedTemp = temp;
  dtostrf(processedTemp, 4, 2, tempChar);
  tempString = tempChar;

  return tempString;
}

String getTurbRawString(){

  float temp;
  float processedTemp;
  char tempChar[100];
  String tempString;

  temp = getTurbidity();
  turbVal = temp;
  processedTemp = temp;
  dtostrf(processedTemp, 4, 2, tempChar);
  tempString = tempChar;

  return tempString;
}