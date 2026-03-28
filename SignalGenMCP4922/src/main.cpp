#include <Arduino.h>
#include <SPI.h>
#include <math.h>
//#include "sinus.h"
#include "sinSweap.h"

double frequency = 120;  //sets the output frequency for the dac
double amplitudeSin = 0.5;

int speedMaximum = 30000000;  //frequency of the SPI Bus
const int CSPin = 10;                   //define const for the SPI chip MCP4922
SPISettings mySetting(speedMaximum, MSBFIRST, SPI_MODE0);  //settings for SPI

void setup() {
  pinMode(CSPin, OUTPUT);           //Sets the chip select pin to output
  SPI.begin();                      //Initilize SPI
  SPI.beginTransaction(mySetting);  //Configures the SPI settings
}

void loop() {
  //spiSin(amplitudeSin, frequency);  //sine wave gen
  double amplitude = 1.0;
  double startFreq = 1.0;
  double endFreq = 10.0;
  double time = 10;
  spiSinSweap(amplitude, startFreq, endFreq, time);
}