void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  analogReadResolution(13);
  Serial.println("Initialized");
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(analogRead(A10) - analogRead(A11));
  delay(100);
  
  

}


void logPin( uint8_t pin){
  Serial.print("Pin ");
  Serial.print(pin);
  Serial.print(": ");
  Serial.println(analogRead(pin));
}
