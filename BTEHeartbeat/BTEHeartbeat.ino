#define BTE Serial4


void setup() {
  BTE.begin(9600);
  Serial.begin(9600);
  pinMode(13, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(13, HIGH);
   
  BTE.println("_*");
  delay(600);

}
