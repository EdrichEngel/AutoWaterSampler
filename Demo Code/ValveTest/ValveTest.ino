#include <ESP32Servo.h>
 
Servo myservo1;  // create servo object to control a servo
Servo myservo2;  // create servo object to control a servo
Servo myservo3;  // create servo object to control a servo
// 16 servo objects can be created on the ESP32
 
float pos = 0;    // variable to store the servo position
// Recommended PWM GPIO pins on the ESP32 include 2,4,12-19,21-23,25-27,32-33 
int servoPin1 = 13;
int servoPin2 = 33;
int servoPin3 = 25;

int valvePos[8] = {10,35,58,83,106,125,150,172};
 
/*
int middle = 90;
int middleOffset = 3;
int left = 130;*/
int middle = 0;
int middleOffset = 0;
int left = 200;



void setup() {
    pinMode(14, OUTPUT);
    digitalWrite(14, HIGH);  // turn the LED on (HIGH is the voltage level)

    pinMode(26, OUTPUT);
 digitalWrite(26, LOW);
	// Allow allocation of all timers
	ESP32PWM::allocateTimer(0);
	ESP32PWM::allocateTimer(1);
	ESP32PWM::allocateTimer(2);
	ESP32PWM::allocateTimer(3);
	myservo1.setPeriodHertz(50);    // standard 50 hz servo
	myservo2.setPeriodHertz(50);    // standard 50 hz servo
	myservo3.setPeriodHertz(50);    // standard 50 hz servo
	myservo1.attach(servoPin1, 500, 2500); // attaches the servo on pin 18 to the servo object
	myservo2.attach(servoPin2, 500, 2500); // attaches the servo on pin 18 to the servo object
	myservo3.attach(servoPin3, 500, 2500); // attaches the servo on pin 18 to the servo object
	// using default min/max of 1000us and 2000us
	// different servos may require different min/max settings
	// for an accurate 0 to 180 sweep
 Serial.begin(115200);
 

}
 
void loop() {

 
    for (int i = 0; i < 8; i++) {
      Serial.println(i);
        myservo1.write(valvePos[i]);
        myservo2.write(valvePos[i]);
        myservo3.write(valvePos[i]);
      Serial.println("Moved");
        delay(3000);
        digitalWrite(26, HIGH);
      Serial.println("Pump On");
        delay(5000);
        digitalWrite(26, LOW);
      Serial.println("Pump Off");
        delay(3000);
    }

    for (int i = 6; i >= 1; i--) {  
       Serial.println(i);
        myservo1.write(valvePos[i]);
        myservo2.write(valvePos[i]);
        myservo3.write(valvePos[i]);
      Serial.println("Moved");
        delay(3000);
        digitalWrite(26, HIGH);
      Serial.println("Pump On");
        delay(5000);
        digitalWrite(26, LOW);
      Serial.println("Pump Off");
        delay(3000);
  }
}