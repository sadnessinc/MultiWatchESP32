#include "serviceWiFi.h"

void WiFiService::begin(const char* ssid, const char* password){
    _ssid = ssid;
    _password = password;
}

bool WiFiService::connect(uint32_t timeoutMs){
    WiFi.mode(WIFI_STA);
    WiFi.begin(_ssid, _password);

    unsigned long start = millis();

    while (WiFi.status() != WL_CONNECTED) 
    {
      if (millis() - start >= timeoutMs) 
      {
        disconnect();
        return false;
      }
      delay(100);
    }

    return true;
}

void WiFiService::disconnect() {
    WiFi.disconnect(true);
    WiFi.mode(WIFI_OFF);
}

bool WiFiService::isConnected(){
    return WiFi.status() == WL_CONNECTED;
}

/*
bool connectWiFi(bool needPrint = false,uint32_t timeoutMs = 10000) {
  Serial.print("Connecting to WiFi");
  if (needPrint) drawLoading("Connecting WiFi");
  unsigned long startAttempt = millis();
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
    {
      if (millis() - startAttempt >= timeoutMs) 
      {
        disconnectWiFi();
        Serial.println("WiFi connecting Error");
        return false;
      }
      delay(100);
    }
  wifiConnected = true;
  Serial.println("WiFi connected");
  return true;
}

void disconnectWiFi(){
  WiFi.disconnect(true);   // разрывает соединение и очищает настройки
  WiFi.mode(WIFI_OFF);     // полностью выключает Wi-Fi радиомодуль
  wifiConnected = false;
}
  */