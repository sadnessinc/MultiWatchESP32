#include "DHTesp.h"
#include "core/WiFi/WebServer/WiFiWebServer.h"

extern DHTesp dht;

unsigned long lastHTDSync = 0;
const unsigned long syncHTDInterval = 2000;
TempAndHumidity dataDHT;

void getDHTdata(){
  unsigned long now = millis();
  if(now - lastHTDSync < syncHTDInterval)return;
  lastHTDSync = now;
  dataDHT = dht.getTempAndHumidity();
  if (isnan(dataDHT.temperature) || isnan(dataDHT.humidity)) return;
  WiFiWebServer::humidity=dataDHT.humidity;
  WiFiWebServer::temperature=dataDHT.temperature;
}



