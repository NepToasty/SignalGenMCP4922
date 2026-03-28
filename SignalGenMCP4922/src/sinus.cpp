/*
#include <Arduino.h>
#include <SPI.h>
#include <math.h>
#include "sinus.h"

//== SPI ==
const int CSPin = 10;                   //define const for the SPI chip MCP4922
const int spiOutputA = 0x0;             //config bit for DAC A output
const int spiOutputB = 0x8000;          //config bit for DAC B output
const int spiVrefBUffered = 0x4000;     //config bit for buffered Vref input
const int spiGainSet_1x = 0x2000;       //config bit for setGain of Vref times 1
const int spiGainSet_2x = 0x0;          //config bit for setGain of Vref times 2
const int spiSHDNAc = 0x1000;           //Active operation of that channel
const int spiSHDNDis = 0x0;             //Disable operation of that channel

double pi = 3.14159;
double vRef = 3.3/2; //Sets the reference of the DAC

uint32_t currentTime = micros();
uint32_t oldTime = 0;

int i = 0;

void spiSin(double Amplitude, int frequencySet){
  uint32_t intervalTime = 1E6 / (360.0 * frequencySet);
  currentTime = micros();
  if(currentTime - oldTime > intervalTime){
    double volt = (vRef + sin((i / 360.0) * 2.0 * pi) * Amplitude) / 2.0;
    int intVolt = (int)(4095.0 * volt / vRef);     //Convertes double volt to int volt
    int toDACData = intVolt + spiOutputA + spiVrefBUffered + spiGainSet_2x + spiSHDNAc; //Configures the SPI with control bits and data
    
    digitalWrite(CSPin, LOW);                         //takes the CS pin low to select the chip
    SPI.transfer16(toDACData);                        //Sends data to the SPI chip
    digitalWrite(CSPin, HIGH);                        //takes the CS pin HIGH to deselect the chip
    Serial.println(volt);
    if(i == 359){
      i = 0;
    }else{
      i++;
    }
    oldTime = currentTime;
  }
}
*/