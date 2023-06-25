
#include <ESP32Servo.h>
#include <OneWire.h>
#include <DallasTemperature.h>


// Define Variables
float servoPos = 0;  // Variable to store the servo position
float turbidityReading = 0;
float temperatureReading = 0;

bool sdCardDetected = 0;
bool sdCardDetectedPrev = 0;

// Define Constants

static const byte onboardLedPin = 2;
static const byte turbidityPin = 12;
static const byte servoPin = 13;
static const byte temperaturePin = 15;
static const byte cardDetectPin = 34;


static const int sToMs = 1000;

static const byte portPos1 = 0;
static const byte portPos2 = 26;
static const byte portPos3 = 51;
static const byte portPos4 = 77;
static const byte portPos5 = 103;
static const byte portPos6 = 129;
static const byte portPos7 = 154;
static const byte portPos8 = 180;

// Define Functions
byte servoDegrees(byte portNumber);

float getTurbidity(float rawADC);
float getTemperature();

// Define Objects
Servo valveServo;                               // Create servo object
OneWire oneWire(temperaturePin);                // Create a OneWire object for the temperature sensor
DallasTemperature temperatureSensor(&oneWire);  // Create a DallasTemperature object by using the DallasTemperature library


void setup() {
  // Start Serial Communication
  Serial.begin(115200);

  // Onboard LED setup
   pinMode(onboardLedPin, OUTPUT);

  // SD Card Breakout Board Setup
   pinMode(cardDetectPin, INPUT);

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
}



  


void loop() {
  // put your main code here, to run repeatedly:

  sdCardDetected = digitalRead(cardDetectPin);
  digitalWrite(onboardLedPin,sdCardDetected);

  if (sdCardDetected != sdCardDetectedPrev){
    if (sdCardDetected){
      Serial.println("SD Card Detected");
    } else{
      Serial.println("SD Card Removed");
    }
  }
  sdCardDetectedPrev = sdCardDetected;

  turbidityReading = getTurbidity(analogRead(turbidityPin));
  temperatureReading = getTemperature();
  Serial.print("Turbidity: ");
  Serial.println(turbidityReading);
  Serial.print("Temperature: ");
  Serial.println(temperatureReading);


  delay(5 * sToMs);
}










byte servoDegrees(byte portNumber) {

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

  // Convert the raw ADC value to the output voltage from the sensor
  voltage = (rawADC * 5 / 4095.0);

  // Convert the output voltage from the sensor to turbidity in NTUs
  return (-1120.4 * voltage * voltage + 5742.3 * voltage - 4352.9);
}

float getTemperature() {
  temperatureSensor.requestTemperatures();
  return temperatureSensor.getTempCByIndex(0);
}
