#include "screenWiFi.h"

#include "core/WiFi/WiFiNetwork.h"
#include "core/AppState/AppState.h"
#include "core/Input/Input.h"
#include "additional/AdditionalLib.h"

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

extern Adafruit_SSD1306 display;
extern InputService input;

void screenWiFi() {
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0,0);
  display.setTextColor(SSD1306_WHITE);
  display.println("WiFi");

  if (wifiNetwork.isConnected()) {
    display.println("Status:ON");
    display.setTextSize(1);
    display.println("Press DOWN to disable");
  } else {
    display.println("Status:OFF");
    display.setTextSize(1);
    display.println("Press DOWN to enable");
  }

  display.display();
}

void updateWiFiInput() {
    if (input.pressed(BTN_BACK)){
        setScreen(MENU);
        return;
    }

  if (input.pressed(BTN_DOWN)) {
    if (wifiNetwork.isConnected()){
      
      drawLoading("DISCONNECTING");
      wifiNetwork.disconnect();}
    else{

      drawLoading("CONNECTING");
      wifiNetwork.connect();}
  }
}