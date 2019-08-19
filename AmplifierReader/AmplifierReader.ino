#include "HX711.h"

#define calibration_factor -7050.0 //This value is obtained using the SparkFun_HX711_Calibration sketch

#define DOUT 1
#define CLK 0
#define LED_PIN 13
#define BUTTON_PIN 11


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



HX711 scale;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  scale.begin(DOUT, CLK);
  scale.set_scale(calibration_factor); //This value is obtained by using the SparkFun_HX711_Calibration sketch
  scale.tare();  //Assuming there is no weight on the scale at start up, reset the scale to 0

}

void loop() {


  checkButton();


  switch (state){
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
  
  Serial.print("Reading: ");
  Serial.print(scale.get_units(), 1); //scale.get_units() returns a float
  Serial.print(" lbs"); //You can change this to kg but you'll need to refactor the calibration_factor
  Serial.println();
  
}





void normal(){
  digitalWrite(LED_PIN, LOW);
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
  state = (state +1)%states;
}
