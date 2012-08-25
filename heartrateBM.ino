//Arduino 1.0+ Compatible only
//Arduino 1.0+ Compatible only
//Arduino 1.0+ Compatible only

// Code to retrieve heartrate information from the Polar Heart Rate Monitor Interface via I2C
// Part: http://www.sparkfun.com/products/8661
// Article:  http://bildr.org/2011/08/heartrate-arduino/

#include "Wire.h"

#define HRMI_I2C_ADDR      127
#define HRMI_HR_ALG        1   // 1= average sample, 0 = raw sample

#define RESTING(heartMode) (heartMode < 3)
#define BRADYCARDIA(heartMode) (heartMode == 0)
#define TACHYCARDIA(heartMode) (heartMode == 7)

#define NONE 0
#define AQUA (1 << 0)
#define ORANGE (1 << 1)
#define RED (1 << 2)

#define WIRE(option, index) (option & (1 << index))

const int ledPin = 13; // LED connected to digital pin 13 
// Variables will change:
int ledState = LOW;             // ledState used to set the LED
long previousMillis = 0;        // will store last time LED was updated
// the follow variables is a long because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long interval = 1000;           // interval at which to blink (milliseconds)
float part1 = 0;

// This holds the pin numbers for the various length
const int MODE_COUNT = 8;
const int WIRE_COUNT = 8;
const int LENGTHS[WIRE_COUNT] = {10, 10, 20, 20, 20, 30, 30, 30};
const int COLORS[WIRE_COUNT] = {AQUA, ORANGE, AQUA, ORANGE, RED, AQUA, ORANGE, RED};
const int PINS[WIRE_COUNT] = {13, 12, 11, 10, 9, 8, 7, 6};

// These are populate by the prepareOutputOptions, based on the settings above
const int OPTION_COUNT = 2 ^ WIRE_COUNT;
int colors_for[OPTION_COUNT] = {0};
int length_for[OPTION_COUNT] = {0};
int mode_options[MODE_COUNT][OPTION_COUNT] = {0};
int mode_options_count[MODE_COUNT] = {0};

void setup(){
  setupHeartMonitor(HRMI_HR_ALG);
  Serial.begin(9600);
  pinMode(ledPin,OUTPUT);
  prepareOutputOptions();
}

void loop(){
  int heartRate = getHeartRate();
  //  Serial.println(heartRate);
  displayHeartRate(heartRate);
  delay(20); //just here to slow down the checking)
}

void prepareOutputOptions() {
  for (int option = 0; option < OPTION_COUNT; option++) {
    int optionLength = 0;
    int optionColors = NONE;
    for (int wireIndex = 0; wireIndex < WIRE_COUNT; wireIndex++) {
      if (WIRE(option, wireIndex)) {
        optionLength += LENGTHS[wireIndex];
        optionColors &= COLORS[wireIndex];
      }
    }
    colors_for[option] = optionColors;
    length_for[option] = optionLength;
  }
  
  for (int mode = 0; mode < MODE_COUNT; mode++) {
    int allowedColors = getAllowedColors(mode);
    int maxLength = getMaxLength(mode);
    int minLength = getMinLength(mode);
    
    for (int option = 0; option < OPTION_COUNT; option++) {
      if (optionFitsColors(option, allowedColors) && optionFitsLengths(option, minLength, maxLength)) {
        markModeOption(mode, option);
      }
    }
  }
}

boolean optionFitsColors(int option, int allowedColors) {
  return (colors_for[option] | allowedColors) == allowedColors;
}

boolean optionFitsLengths(int option, int minLength, int maxLength) {
  int length = length_for[option];
  return (length >= minLength && length <= maxLength);
}

void markModeOption(int mode, int option) {
  mode_options[mode][mode_options_count[mode]] = option;
  mode_options_count[mode] += 1;
}

void writeLEDfrequency(long interval){
  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis > interval) {
    // save the last time you blinked the LED 
    previousMillis = currentMillis;   
    // if the LED is off turn it on and vice-versa:
    if (ledState == LOW)
      ledState = HIGH;
    else
      ledState = LOW;
    // set the LED with the ledState of the variable:
    digitalWrite(ledPin, ledState);
  }  
}

void setupHeartMonitor(int type){
  //setup the heartrate monitor
  Wire.begin();
  writeRegister(HRMI_I2C_ADDR, 0x53, type); // Configure the HRMI with the requested algorithm mode
}

void displayHeartRate(int heartRate) {
  int displayMode = getDisplayMode(heartRate);
  int option = selectOption(displayMode);
  activateLights(option);
}

int selectOption(int displayMode) {
  return mode_options[displayMode][rand() % mode_options_count[displayMode]];
}

void activateLights(int option) {
  for (int wireIndex = 0; wireIndex < WIRE_COUNT; wireIndex++) {
    digitalWrite(PINS[wireIndex], WIRE(option, wireIndex) ? HIGH : LOW);
  }
}

int getDisplayMode(int heartRate) {
  if (heartRate < 60) {
    return 0;
  } else if (heartRate >= 120) {
    return 7;
  } else {
    // 60-120 -> 1-6
    return (heartRate / 10) - 5;
  }
}

int getMinLength(int displayMode) {
  if (RESTING(displayMode) || TACHYCARDIA(displayMode)) {
    return 10;
  } else {
    // translate 3-6 -> 10-40
    return (displayMode - 2) * 10;
  }
}

int getMaxLength(int displayMode) {
  // displayMode is 0-7. Target length is 10-80 feet
  return (displayMode * 10) + 10;
}

int getAllowedColors(int displayMode) {
  if (BRADYCARDIA(displayMode)) {
    return AQUA;
  } else if (RESTING(displayMode)) {
    return AQUA & ORANGE;
  } else if ((displayMode < 5) || TACHYCARDIA(displayMode)) {
    return AQUA & ORANGE & RED;
  } else {
    return ORANGE & RED;
  }
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
