#include "screenTime.h"

#include "additional/AdditionalLib.h"
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <Arduino.h>

#include "core/AppState/AppState.h"
#include "core/Input/Input.h"
#include "core/WiFi/WiFiNetwork.h"
#include "core/DateTime/DateTime.h"

extern Adafruit_SSD1306 display;
extern InputService input;

enum TimeScreenState {
    IDLE,
    WIFI_CONNECTING,
    SYNCING,
    SHOW_TIME,
    ERROR
};

void screenTime() {

    display.clearDisplay();

    TimeState state = getTimeState();

    if (state == TIME_WIFI_CONNECTING) {
        display.setTextSize(2);
        printCenter("WiFi...");
        drawWiFi();
        display.display();
        return;
    }

    if (state == TIME_WAITING) {
        display.setTextSize(2);
        printCenter("Sync...");
        drawWiFi();
        display.display();
        return;
    }

    if (state == TIME_ERROR) {
        display.setTextSize(2);
        printCenter("No NTP");
        drawWiFi();
        drawLastSync();
        display.display();
        return;
    }

    if (state == TIME_READY) {

        struct tm timeinfo;

        if (getCurrentTime(timeinfo)) {

            display.setTextSize(4);

            char buf[6];
            sprintf(buf, "%02d:%02d",
                    timeinfo.tm_hour,
                    timeinfo.tm_min);

            printCenter(buf);
        }

        drawWiFi();
        drawLastSync();
        display.display();
    }
}

void updateTimeInput(){
    if(input.pressed(BTN_BACK)){
        setScreen(MENU);
        return;
    }
}