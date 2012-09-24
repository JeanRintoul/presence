//Arduino 1.0+ Compatible only
//Arduino 1.0+ Compatible only
//Arduino 1.0+ Compatible only

// Code to retrieve heartrate information from the Polar Heart Rate Monitor Interface via I2C
// Part: http://www.sparkfun.com/products/8661
// Article:  http://bildr.org/2011/08/heartrate-arduino/

#include "Wire.h"

#define HRMI_I2C_ADDR      127
#define HRMI_HR_ALG        1   // 1= average sample, 0 = raw sample
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);


const int ledPin = 13; // LED connected to digital pin 13 
// Variables will change:
int ledState = LOW;             // ledState used to set the LED
long previousMillis = 0;        // will store last time LED was updated
// the follow variables is a long because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long interval = 1000;           // interval at which to blink (milliseconds)
float part1 = 0;

void setup(){
  setupHeartMonitor(HRMI_HR_ALG);
  Serial.begin(9600);
  pinMode(ledPin,OUTPUT);
  
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  lcd.setCursor(0, 1);
  // Print a message to the LCD.
  lcd.print("beats per minute!");  
}

void loop(){

  int heartRate = getHeartRate();
  Serial.println(heartRate);

  // This is the desired LED blink interval: 
  interval = 60000/heartRate;
  // write to pin 13, 4.83 v coming out of there in time with pulse. 
  writeLEDfrequency();
  
  // Now, we need some LCD screen output code... 
  lcd.setCursor(0, 0);
  // print the number of seconds since reset:
  lcd.print(heartRate);
  delay(interval);
}

void writeLEDfrequency(){
  // if the LED is off turn it on and vice-versa:
  if (ledState == LOW)
    ledState = HIGH;
  else
    ledState = LOW;
  // set the LED with the ledState of the variable:
  digitalWrite(ledPin, ledState);
}

void setupHeartMonitor(int type){
  //setup the heartrate monitor
  Wire.begin();
  writeRegister(HRMI_I2C_ADDR, 0x53, type); // Configure the HRMI with the requested algorithm mode
}

int getHeartRate(){
  //get and return heart rate
  //returns 0 if we couldnt get the heart rate
  byte i2cRspArray[3]; // I2C response array
  i2cRspArray[2] = 0;

  writeRegister(HRMI_I2C_ADDR,  0x47, 0x1); // Request a set of heart rate values 

  if (hrmiGetData(127, 3, i2cRspArray)) {
    return i2cRspArray[2];
  }
  else{
    return 0;
  }
}

void writeRegister(int deviceAddress, byte address, byte val) {
  //I2C command to send data to a specific address on the device
  Wire.beginTransmission(deviceAddress); // start transmission to device 
  Wire.write(address);       // send register address
  Wire.write(val);         // send value to write
  Wire.endTransmission();     // end transmission
}

boolean hrmiGetData(byte addr, byte numBytes, byte* dataArray){
  //Get data from heart rate monitor and fill dataArray byte with responce
  //Returns true if it was able to get it, false if not
  Wire.requestFrom(addr, numBytes);
  if (Wire.available()) {

    for (int i=0; i<numBytes; i++){
      dataArray[i] = Wire.read();
    }

    return true;
  }
  else{
    return false;
  }
}
