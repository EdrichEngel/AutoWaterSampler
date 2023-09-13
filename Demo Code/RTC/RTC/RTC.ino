#include <Wire.h>           // Include the Wire library for I2C communication
#include "RTClib.h"         // Include the RTC library

RTC_DS3231 rtc;             // Create an RTC object

void setup() {
  Serial.begin(115200);     // Start the serial communication
  Wire.begin();             // Start the I2C communication

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, lets set the time!");
    // If the RTC lost power, set the time to a default value (change as needed).
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  // Uncomment this line to set the RTC with an explicit date & time:
  // rtc.adjust(DateTime(2023, 9, 9, 12, 0, 0)); // Set the date and time here
}

void loop() {
  DateTime now = rtc.now();    // Read the current date and time from the RTC

  // Print the date and time in a human-readable format
  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(" ");
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();

  delay(1000); // Wait for a second before reading the time again
}
