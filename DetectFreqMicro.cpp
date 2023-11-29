#include "arduinoFFT.h"
#include <iostream>
#include <regex>
#include <vector>
#include <string>
#include <ESP8266WiFi.h>
#define SAMPLES 256//256
#define SAMPLING_FREQUENCY 10000

arduinoFFT FFT = arduinoFFT();

unsigned int sampling_period_us;
unsigned long microseconds;
double vReal[SAMPLES];
double vImag[SAMPLES];
unsigned long newTime, oldTime;
/*detectar la presencia de al menos 2 "A" seguidas de al menos 2 "B" o al menos 2 "B"
 seguidas de al menos 2 "A" en cualquier parte del string.*/
std::regex hi_lo("A{2,}B{2,}|B{2,}A{2,}", std::regex_constants::ECMAScript | std::regex_constants::icase);
std::vector<float>  signals;

float FA_MIN=700;//700
float FA_MAX=1500;//1500
float FB_MIN=200;//200
float FB_MAX=650;//600

void setup() {
  Serial.begin(115200);
  pinMode(2,OUTPUT);
  digitalWrite(2,HIGH);// led integrado 
  sampling_period_us = round(1000000 * (1.0 / SAMPLING_FREQUENCY));
}

void loop() {
  for (int i = 0; i < SAMPLES; i++) {
    newTime = micros() - oldTime;
    oldTime = newTime;
    vReal[i] = analogRead(A0);
    vImag[i] = 0;
    while (micros() < (newTime + sampling_period_us)) { /* do nothing to wait */ }
  }
  FFT.Windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
  FFT.Compute(vReal, vImag, SAMPLES, FFT_FORWARD);
  FFT.ComplexToMagnitude(vReal, vImag, SAMPLES);
  
  for (int i = 2; i < (SAMPLES / 2); i++) {
    if (vReal[i]>200) {//60
      std::cout<<"Frecuencia:"<<i * (SAMPLING_FREQUENCY / SAMPLES)<<" Hz, amplitud: "<<vReal[i]<<std::endl;
      //detectar si el frame de 20 tonos tiene una sirena 
      if(signals.size()==20){
        detectSiren(signals);
        signals.clear();
      }else{
        //llenamos 
        signals.push_back(i * (SAMPLING_FREQUENCY / SAMPLES));
      }
    }
  }
  
}
void detectSiren(std::vector<float>signals){
  if(regex_search(get_hilo(signals),hi_lo)){
    digitalWrite(2,LOW);
    std::cout<<"Sirena detectada "<<get_hilo(signals)<<std::endl;
    delay(5000);
    digitalWrite(2,HIGH);
    ESP.restart();// evitar lecturas incorrectas de frecuencias 
  }else{
    digitalWrite(2,HIGH);
    std::cout<<"no detectada "<<get_hilo(signals)<<std::endl;
  }
}
bool isFa(float freq) {return (FA_MIN <= freq && freq <= FA_MAX);}
bool isFb(float freq) {return (FB_MIN <= freq && freq <= FB_MAX);}

std::string get_hilo(std::vector<float> freqs){
  std::string patron="";
  for(float freq: freqs){
    if(isFa(freq)) patron+="A";
    else if(isFb(freq)) patron+="B";
    else patron+="-";
  }
  return patron;
}
