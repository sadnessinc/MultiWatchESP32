#include "DateTime.h"

#include <Arduino.h>
#include "core/WiFi/WiFiNetwork.h"

#include "additional/AdditionalLib.h"
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
extern Adafruit_SSD1306 display;

const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 3 * 3600;
const int daylightOffset_sec = 0;

const unsigned long syncInterval = 3600000;

static TimeState state = TIME_IDLE;
unsigned long lastSync = 0;
static bool ntpRequested = false;
static unsigned long waitStart = 0;

bool syncedOnce = false;
struct tm lastSyncTime;

void timeUpdate() {

    unsigned long now = millis();

    switch (state) {

        case TIME_IDLE:
            if (!wifiNetwork.isConnected()) {
                wifiNetwork.connect();
                
                state = TIME_WIFI_CONNECTING;
                break;
            }

            wifiNetwork.keepAlive();
            configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
            waitStart = now;
            state = TIME_WAITING;
            break;


        case TIME_WIFI_CONNECTING:
            if (wifiNetwork.isConnected()) {
                state = TIME_IDLE;
            }
            break;


        case TIME_WAITING: {
            struct tm timeinfo;
            if (getLocalTime(&timeinfo, 0)){
                lastSync = now;
                state = TIME_READY;
                syncedOnce = true;
                lastSyncTime = timeinfo;
                break;
            }

            // быстрый таймаут 3 секунды вместо 10
            if (now - waitStart > 3000) {
                state = TIME_ERROR;
            }
        }
        break;


        case TIME_READY:
            if (now - lastSync > syncInterval) {
                state = TIME_IDLE;
            }
            break;


        case TIME_ERROR:
            // не зависаем — просто ждём следующей попытки
            if (now - waitStart > 5000) {
                state = TIME_IDLE;
            }
            break;
    }
}

TimeState getTimeState() {
    return state;
}

bool getCurrentTime(struct tm &info) {
    return getLocalTime(&info);
}

bool getCurrentTimeHHMM(char* out, size_t outSize) {
    if (!out || outSize < 6) return false; // "HH:MM" + '\0'

    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
        out[0] = '\0';
        return false;
    }

    snprintf(out, outSize, "%02d:%02d", timeinfo.tm_hour, timeinfo.tm_min);
    return true;
}