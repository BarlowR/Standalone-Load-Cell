#include "HX711.h"
#include <SD.h>
#include <SPI.h>
#include <EEPROM.h>


#define calibration_factor -7050.0 //This value is obtained using the SparkFun_HX711_Calibration sketch

#define DOUT 1
#define CLK 0
#define LED_PIN 13
#define BUTTON_PIN 11
#define FILELOG 1

//error variables
bool LED = 0;
IntervalTimer blinker;



//debouncing variables
int elapsedTime = 0;
bool checking = false;
int pressTime = 1000;

//state variables
const int states = 2;
int state = 0;
/*  state 0 -> No Recording
 *  state 1 -> Recording
 */
bool recording = false;


//SD Card

const int chipSelect = BUILTIN_SDCARD;
int fileNum = 0;



HX711 loadCell;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  digitalWrite(LED_PIN, HIGH);
  incrementFileNum();
  setupSDCard();
  setupLoadCell();
  delay(2000);
  digitalWrite(LED_PIN, LOW);
  
}

void loop() {


  checkButton();


  switch (state){
    case -1:
      LED = !LED;
      digitalWrite(LED_PIN, LED);
      delay(500);
      break;
    case 0: 
      normal();
      break;
    case 1: 
      record();
      break;
    default:
      Serial.println("Case Error");
      break;
  }
}



void record(){

  digitalWrite(LED_PIN, HIGH);

  if (!recording)
  {
    recording = true;
    incrementFileNum();
  }
  else
  {
     // make a string for assembling the data to log:
    String dataString = String(fileNum); 
  
    // grab a timestamp
  
    unsigned long t = millis();
    int tim[2];
    tim[0] = (t/1000)%60;
    tim[1] = (t/60000)%60;
    
    dataString += ",";
  
    for (int i = 1; i >=0; i--){
       if (tim[i] < 10) dataString += "0";
       dataString +=  tim[i];
      if (i >0) dataString += ":";
      else dataString += ",";
    }

    dataString += String((loadCell.read() +560614) * 0.000107) + ",";
    dataString += String(loadCell.read());
     
    
    File dataFile = SD.open("dataLog.csv", FILE_WRITE);
  
    // if the file is available, write to it:
    if (dataFile) {
      dataFile.println(dataString);
      dataFile.close();
      // print to the serial port too:
      Serial.println(dataString);
    }  
    // if the file isn't open, pop up an error:
    else {
      Serial.println("error opening datalog.csv");
      state = -1;
    }  
  }
  
}



void normal(){
  digitalWrite(LED_PIN, LOW);
  recording = false;
}

void incrementFileNum(){
  fileNum = (int)EEPROM.read(FILELOG);
  EEPROM.write(FILELOG, fileNum+1);
}



void setupSDCard(){
  Serial.print("Initializing SD card...");
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    state = -1;
    return;
  }
  Serial.println("card initialized.");
}

void setupLoadCell(){
  loadCell.begin(DOUT, CLK);
  loadCell.set_scale(calibration_factor); //This value is obtained by using the SparkFun_HX711_Calibration sketch
  loadCell.tare();  //Assuming there is no weight on the scale at start up, reset the scale to 0
}







void checkButton(){
   if (!digitalRead(BUTTON_PIN)){
    if (!checking) 
    {
      elapsedTime = millis();
      checking = true;
    }
    else 
    {
      if (millis() - elapsedTime> pressTime) {
        if (!digitalRead(BUTTON_PIN)){
          Serial.println("switched");

          incrementState();
          
          checking = false;
        }
      }
    }
  }
  else if (checking && millis() - elapsedTime> pressTime)
  {
    Serial.println("bypass");
    checking = false;
  }
}




void incrementState(){
  if (state != -1) state = (state +1)%states;
}
