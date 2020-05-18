/*
 *  Microcontroller module for the ventilator project developed by EASY Engineering and Systems Group
 *  Instituto de Computação - Universidade Federal de Alagoas
 *  @author glauberrleite
 *  @author vangasse
 */

#ifndef ARDUINO_H
#define ARDUINO_H
#include <Arduino.h>
#endif

#include "sensors.h"
#include "valves.h"

int cnt = 0;
Sensors sensors;
Valves valves;

// Custom functions

String readStringSerial(){
  String content = "";
  char character;
  
  // Enquanto receber algo pela serial
  while(Serial.available() > 0) {
    // Lê byte da serial
    character = Serial.read();
    // Ignora caractere de quebra de linha
    if (character != '\n'){
      // Concatena valores
      content.concat(character);
    }
    // Aguarda buffer serial ler próximo caractere
    delay(10);
  }    
  return content;
}

String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;

  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }

  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}

// Main setup and loop
void setup() {
  Serial.begin(9600);

  sensors = Sensors();
  valves = Valves();
}

void loop() {

  if (cnt > 100) {
    cnt = 0; 
    sensors.update();

    Serial.print(sensors.getFL_INT());
    Serial.print("\t\t");
    Serial.print(sensors.getFL_PAC_INS());
    Serial.print("\t\t");
    Serial.print(sensors.getFL_PAC_EXP());
    Serial.print("\t\t");
    Serial.print(sensors.getPRES_PAC());
    Serial.print("\t\t");
    Serial.print(sensors.getPRES_INT());

    Serial.println();
  }
  
  // Expiratory Valve Test

  if (Serial.available() > 0) {
    // Lê toda string recebida
    String received = readStringSerial();
  
    String part01 = getValue(received, ',', 0);
    String part02 = getValue(received, ',', 1);

    float value = part02.toFloat();

    if (part01.equals("AUTO")) {
      valves.setAUTO_SEC_VALVE(bool(value));
    } else if (part01.equals("MANUAL")) {
      valves.setMANUAL_SEC_VALVE(bool(value));
    } else if (part01.equals("INS")) {
      valves.setINS_VALVE(value);
    } else {
      valves.setEXP_VALVE(value);
    }

    Serial.print(part01);
    Serial.print('\t');
    Serial.println(part02);
  
  }

  cnt++;
  delay(10);
}