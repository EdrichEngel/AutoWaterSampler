#include "esp_wifi.h"
// Factor to convert seconds to micro seconds
#define S_TO_uS_FACTOR 1000000

// Time for system to be in deep sleep mode
#define TIME_TO_SLEEP  10

RTC_DATA_ATTR int bootCount = 0;

void print_wakeup_reason(){
   esp_sleep_wakeup_cause_t wake_up_source;

   wake_up_source = esp_sleep_get_wakeup_cause();

   switch(wake_up_source){
      case ESP_SLEEP_WAKEUP_EXT0 : Serial.println("Wake-up from external signal with RTC_IO"); break;
      case ESP_SLEEP_WAKEUP_EXT1 : Serial.println("Wake-up from external signal with RTC_CNTL"); break;
      case ESP_SLEEP_WAKEUP_TIMER : Serial.println("Wake up caused by a timer"); break;
      case ESP_SLEEP_WAKEUP_TOUCHPAD : Serial.println("Wake up caused by a touchpad"); break;
      default : Serial.printf("Wake up not caused by Deep Sleep: %d\n",wake_up_source); break;
   }
}

void setup(){
   Serial.begin(115200);

   ++bootCount;
   Serial.println("----------------------");
   Serial.println(String(bootCount)+ "th Boot ");

   // Displays the reason for the wake up
   print_wakeup_reason();

   //Timer Configuration
   esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * S_TO_uS_FACTOR);
   Serial.println("ESP32 wake-up in " + String(TIME_TO_SLEEP) + " seconds");

   // Go in Deep Sleep mode
   Serial.println("Goes into Deep Sleep mode");
   Serial.println("----------------------");
   //esp_bluedroid_disable();
   //esp_bt_controller_disable();
   //esp_wifi_stop();
   delay(100);
   esp_deep_sleep_start();
   Serial.println("This will never be displayed");
}

void loop(){
}