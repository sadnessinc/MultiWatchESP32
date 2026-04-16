#include "DateTime.h"

#include "Arduino.h"
#include "services\WiFi\instance\instanceServiceWiFi.h"
#include "additional\AdditionalLib.h"


//DATE-TIME SYNC
const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 3 * 3600; // МСК +3

const unsigned long syncInterval = 3600000; // раз в час
const int daylightOffset_sec = 0; 

struct tm lastSyncTime;
bool syncedOnce = false;
bool lastSyncTimeChanged = false;
unsigned long lastSync = 0;

bool syncTime (bool needPrint, uint32_t timeoutMs){
  if (needPrint) drawLoading("Syncing");
  if (!wifiService.isConnected()) return false;
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  struct tm timeinfo;
  if (getLocalTime(&timeinfo, timeoutMs)) {
        lastSyncTime = timeinfo;
        syncedOnce = true;
        lastSync = millis();
        lastSyncTimeChanged = true;
        return true;
  }
  return false;
}

void checkLastSync(){
  unsigned long now = millis();
  // периодическая синхронизация раз в syncInterval
  if (now - lastSync > syncInterval) 
  {
    if(!wifiService.isConnected()){
        wifiService.connect();
    }
    if(wifiService.isConnected()){
      if(syncTime()){
        wifiService.disconnect();
      }
    }
    }
}

