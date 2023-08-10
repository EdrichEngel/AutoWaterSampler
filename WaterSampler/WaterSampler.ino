
#include <ESP32Servo.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "FS.h"
#include "SD.h"
#include "SPI.h"
#include "cmath"


// Define Variables
float servoPos = 0;  // Variable to store the servo position
float turbidityReading = 0;
float temperatureReading = 0;

bool sdCardDetected = 0;
bool sdCardDetectedPrev = 0;
bool delayFlag = 0;
bool fileCreatedOnStartup = 1;
bool failedToReadFile = 0;

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
String sampleLocation = "N/A";
String sampleDate = "N/A";
String sampleCountString = "N/A";

RTC_DATA_ATTR int bootCycleCount = 0;
RTC_DATA_ATTR int sampleCurrent = 1;

esp_sleep_wakeup_cause_t wakeup_reason;

// Define Constants

static const byte onboardLedPin = 2;
static const byte turbidityPin = 27;
static const byte servoPin = 13;
static const byte temperaturePin = 4;
static const byte cardDetectPin = 34;


static const int sToMs = 1000;
static const int sToUs = 1000000;
static const int msToUs = 1000;

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

float getTurbidity(float rawADC);
float getTemperature();

void initSDCard();
void appendFile(fs::FS &fs, String path, String message);
void readSettingsFile(fs::FS &fs, String path);
void decodeSDString(String dataString);
void splitStrings(String data);
void writeGeneralInfoToSD();
void duplicateFileDetected();
void print_wakeup_reason();
void startDeepSleep();
void deepSleepSetup(int Time);

// Define Objects
Servo valveServo;                               // Create servo object
OneWire oneWire(temperaturePin);                // Create a OneWire object for the temperature sensor
DallasTemperature temperatureSensor(&oneWire);  // Create a DallasTemperature object by using the DallasTemperature library


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
void IRAM_ATTR sdCardInsert() {
  delayFlag = 0;
  duplicateFileCount = 0;
  fileCreatedOnStartup = 0;
}

void setup() {
  // Start Serial Communication
  Serial.begin(115200);

  //Print boot cycle number and the wakeup reason
  Serial.println("Boot number: " + String(bootCycleCount));
  print_wakeup_reason();


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


  if (bootCycleCount == 0) {
    while (testFileExistance(SD, fileNameData)) {
      duplicateFileDetected();
    }

    if (!testFileExistance(SD, fileNameData)) {
      Serial.println("Writing general info");
      writeGeneralInfoToSD();
      state = 1;
    } else {
      Serial.println("Error while creating duplicate file.");
      state = 0;
    }
  }
  bootCycleCount++;
  deepSleepSetup(frequency);
}








void loop() {
  // put your main code here, to run repeatedly:

  if (sampleCount == 0) {
    state = 0;
  } else {
    state = 1;
  }
  if ((sampleCurrent <= sampleCount) || (sampleCount == 0)) {


    //if (delayFlag == 0) {

    sdCardDetected = digitalRead(cardDetectPin);
    digitalWrite(onboardLedPin, sdCardDetected);

    if ((sdCardDetected != sdCardDetectedPrev) || (failedToReadFile)) {
      if (sdCardDetected) {
        Serial.println("SD Card Detected");
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
        Serial.println("SD Card Removed");
        frequency = 1000;
        sdInit = 0;
        sampleCurrent = 1;
        sampleCount = 0;
      }
    }
    sdCardDetectedPrev = sdCardDetected;

    turbidityReading = getTurbidity(analogRead(turbidityPin));
    temperatureReading = getTemperature();
    Serial.print("Sample ");
    Serial.print(sampleCurrent);
    Serial.print(" of ");
    Serial.println(sampleCount);
    Serial.print("Turbidity: ");
    Serial.println(turbidityReading);
    Serial.print("Temperature: ");
    Serial.println(temperatureReading);
    Serial.println(" ");

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
        Serial.println("System has finished sampling");
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
      Serial.println("System has finished sampling");
    }
    sampleCount = -1;
    deepSleepSetup(-1);
    startDeepSleep();

    //Serial.println("Stuck");
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
float getTurbidity(float rawADC) {
  float voltage;
  float turb;

  //Serial.print("raw: ");
  //Serial.println(rawADC);
  // Convert the raw ADC value to the output voltage from the sensor
  voltage = (float)(rawADC * 5 / 4095.0 * 0.8776 + 0.412);
  /* Serial.print("VOLTAGE: ");
  Serial.println(voltage);*/
  // Convert the output voltage from the sensor to turbidity in NTUs
  turb = (-1120.4 * voltage * voltage + 5742.3 * voltage - 4352.9);
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
    Serial.println("SD Card Mount Failed");
    sdError = 1;
    return;
  }
  Serial.println("SD Card Mounted");
  if (sdCardType == CARD_NONE) {
    Serial.println("No SD card attached");
    sdError = 1;
    return;
  } else {
    sdInit = 1;
    Serial.println("SD Card Attached");
  }
}


// Using function written for SD card example
void appendFile(fs::FS &fs, String path, String message) {
  //Serial.printf("Appending to file: %s\n", path.c_str());

  File file = fs.open(path.c_str(), FILE_APPEND);
  if (!file) {
    Serial.println("Failed to open file for appending");
    return;
  }
  if (file.print(message.c_str())) {
    //Serial.println("Message appended");
  } else {
    Serial.println("Append failed");
  }
  file.close();
}


bool testFileExistance(fs::FS &fs, String path) {

  Serial.printf("Testing Existance Of: %s\n", path.c_str());

  File file = fs.open(path.c_str());
  if (!file) {
    Serial.println("File Does Not Exists");
    return 0;
  } else {
    Serial.println("File Exists");
    return 1;
  }
}


void readSettingsFile(fs::FS &fs, String path) {
  String sdDataRead;

  //initSDCard();

  Serial.printf("Reading Settings From: %s\n", path.c_str());

  File file = fs.open(path.c_str());
  if (!file) {
    Serial.println("Failed to open file for reading");
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
    Serial.println("Writing general info");
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
    Serial.print("File Name Changed To: ");
    fileNameData = "/" + fileNameData;
    Serial.println(fileNameData);

  } else {
    if (strcmp(dataString.substring(0, 9).c_str(), "Frequency") == 0) {
      // Frequency
      durationString = dataString.substring(10, dataString.length() - 2);
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

      Serial.print("Frequency Changed To: ");
      Serial.println(frequency);
    } else {
      if (strcmp(dataString.substring(0, 8).c_str(), "Location") == 0) {
        sampleLocation = dataString.substring(9, dataString.length() - 1);
        Serial.print("Sample Location: ");
        Serial.println(sampleLocation);
      } else {
        if (strcmp(dataString.substring(0, 4).c_str(), "Date") == 0) {
          sampleDate = dataString.substring(5, dataString.length() - 1);
          Serial.print("Sample Date: ");
          Serial.println(sampleDate);
        } else {
          if (strcmp(dataString.substring(0, 11).c_str(), "SampleCount") == 0) {
            sampleCountString = dataString.substring(12, dataString.length());
            sampleCount = sampleCountString.toInt();
            Serial.print("Sample Count: ");
            Serial.println(sampleCount);
          } else {
            Serial.println("Error occured while decoding the settings file.");
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
    sampleCountString = "N/A";
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
  Serial.print("Duplicate file found: ");
  Serial.println(duplicateFileCount);
  Serial.println(fileNameData);
  Serial.println("");
}

void print_wakeup_reason() {
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();

  switch (wakeup_reason) {
    case ESP_SLEEP_WAKEUP_EXT0: Serial.println("Wakeup caused by external signal using RTC_IO"); break;
    case ESP_SLEEP_WAKEUP_EXT1: Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
    case ESP_SLEEP_WAKEUP_TIMER: Serial.println("Wakeup caused by timer"); break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD: Serial.println("Wakeup caused by touchpad"); break;
    case ESP_SLEEP_WAKEUP_ULP: Serial.println("Wakeup caused by ULP program"); break;
    default: Serial.printf("Wakeup was not caused by deep sleep: %d\n", wakeup_reason); break;
  }
}


void startDeepSleep() {
  Serial.println("Going to sleep now");
  delay(1000);
  Serial.flush();
  esp_deep_sleep_start();
  Serial.println("This will never be printed");
}

void deepSleepSetup(int Time) {
  if (Time != -1) {
    esp_sleep_enable_timer_wakeup(Time * msToUs);
    Serial.println("Setup ESP32 to sleep for every " + String(frequency / 1000) + " Seconds");
  } else {
    esp_sleep_disable_wakeup_source(ESP_SLEEP_WAKEUP_TIMER);
  }
}
