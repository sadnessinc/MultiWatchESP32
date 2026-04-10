#include "screenWiFi.h"

#include "services\WiFi\instance\instanceServiceWiFi.h"
#include "core/AppState/AppState.h"
#include "core/Input/Input.h"

#include <Arduino.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

extern Adafruit_SSD1306 display;

InputService input;

void screenWiFi() {
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0,0);
  display.setTextColor(SSD1306_WHITE);
  display.println("WiFi");

  if (wifiService.isConnected()) {
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
        currentScreen = MENU;
        return;
    }

  if (input.pressed(BTN_DOWN)) {
    if (wifiService.isConnected()){
      
      // заменить на drawLoading
        display.clearDisplay();
      display.setTextSize(2);
      display.setCursor(0,16);
      display.setTextColor(SSD1306_WHITE);
      display.print("DISCONNECTING");
      display.display();


      wifiService.disconnect();}
    else{
        // заменить на drawLoading
      display.clearDisplay();
      display.setTextSize(2);
      display.setCursor(0,16);
      display.setTextColor(SSD1306_WHITE);
      display.print("CONNECTING");
      display.display();
      wifiService.connect();}
  }
}