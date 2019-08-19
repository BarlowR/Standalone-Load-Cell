
#include <SD.h>
#include <SPI.h>

#include <EEPROM.h>


const int chipSelect = BUILTIN_SDCARD;

IntervalTimer dataLog;

int fileNum = 0;

const byte fileAddress = 1;

void setup()
{
  fileNum = (int)EEPROM.read(fileAddress);
  EEPROM.write(fileAddress, fileNum+1);

  analogReadResolution(16);

  Serial.begin(9600);
  Serial.print("Initializing SD card...");
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    return;
  }
  Serial.println("card initialized.");

  dataLog.begin(writeData, 1000000);

}

void loop()
{
  
}


void writeData(){
// make a string for assembling the data to log:
  String dataString = String(fileNum); 

  // read three sensors and append to the string:

  unsigned long t = millis();
  int tim[3];
  tim[0]= (t%1000);
  tim[1] = (t/1000)%60;
  tim[2] = (t/60000)%60;
  
  dataString += ",";

  for (int i = 2; i >=0; i--){
     dataString +=  tim[i];
    if (i >0) dataString += ":";
    else dataString += ",";
  }

  
  
  dataString+= analogRead(A10);
  dataString += ",";
  dataString+= analogRead(A11);
  
+

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
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
    Serial.println("error opening datalog.txt");
  } 
}
