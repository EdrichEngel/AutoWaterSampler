

#define pinSwitchMode 10 // Change.......................................................................................
byte stateSwitchMode = 0;

String serialDataReceived = "";
String serialDataProcessed = "";
bool serialDataReceivedState = false;

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


void setup() {
  // put your setup code here, to run once:
  pinMode(pinSwitchMode, INPUT);

   Serial.begin(115200);
   Serial.println("Serial Started!");

}

void loop() {
  

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

void monitorMode(){

  readSerial();

  if(serialDataReceivedState == true){

    if (serialDataReceived[1] == '#'){
      //2.4;2.5;2.6
      switch(serialDataReceived[0]){
        case 'R':
          guiReadCoefficients();
        break;
        case 'C':
          guiNewCoefficients();
        break;
        case 'S': 
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

void collectionMode(){

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

}

void guiNewCoefficients(){

}

void guiReadSensorValue(){

}

void guiMonitor(){

}

void guiFileName(){

}

void guiFileDataRead(){

}

void guiConfig(){

}

void guiNewConfig(){

}


