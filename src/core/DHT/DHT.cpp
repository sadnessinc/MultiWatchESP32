#include "DHT.h"
#include "DHTesp.h"

extern DHTesp dht;

unsigned long lastHTDSync = 0;
const unsigned long syncHTDInterval = 2000;
TempAndHumidity dataDHT;

void getDHTdata(){
  unsigned long now = millis();
  if(now - lastHTDSync < syncHTDInterval)return;
  lastHTDSync = now;
  dataDHT = dht.getTempAndHumidity();
}



