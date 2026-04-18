#include "AdditionalLib.h"

#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <Arduino.h>
#include "config/Pins.h"

#include "core/WiFi/WiFiNetwork.h"

#include "core/DateTime/DateTime.h"

extern Adafruit_SSD1306 display;


extern bool syncedOnce;
extern bool lastSyncTimeChanged;
extern unsigned long lastSync;
extern tm lastSyncTime;

const unsigned char wifiIcon [] PROGMEM = {
	0x7e, 0x81, 0x3c, 0x42, 0x18, 0x24, 0x00, 0x18
};
const unsigned char timeIcon [] PROGMEM = {
	0x3c, 0x52, 0x91, 0x95, 0x99, 0x81, 0x42, 0x3c
};

const unsigned char syncIcon [] PROGMEM = {
	0x3e, 0x44, 0x80, 0x81, 0x81, 0x01, 0x22, 0x7c
};

const unsigned char crossIcon [] PROGMEM = {
	0x81, 0x42, 0x24, 0x18, 0x18, 0x24, 0x42, 0x81
};


void drawWiFiIcon() {
  // Размер и позиция
  int x = SCREEN_WIDTH - 8; //left time
  int y = 0;

  if (wifiNetwork.isConnected()) {
    display.drawBitmap(x, y, wifiIcon, 8, 8, SSD1306_WHITE);
  } else {
    // Если не подключено — можно стереть иконку
    display.fillRect(x, y, 8, 8, SSD1306_BLACK);
  }
}

void drawLastSync(){
    if(!syncedOnce){
      int x = 2;  
      int y = 0;     
      display.drawBitmap(x, y, syncIcon, 8, 8, SSD1306_WHITE);
      x = 12;
      display.drawBitmap(x, y, crossIcon, 8, 8, SSD1306_WHITE);
      return;
    }


    int x = 2;
    int y = 0;
    display.setCursor(x, y);
    display.drawBitmap(x, y, syncIcon, 8, 8, SSD1306_WHITE);

    char buf[6];
    snprintf(buf, sizeof(buf), "%02d:%02d", lastSyncTime.tm_hour, lastSyncTime.tm_min);

    display.setTextSize(1);         // маленький шрифт

    x = 12;
    display.setCursor(x, y);
    display.setTextColor(SSD1306_WHITE);
    display.print(buf);
    
}

Bounds calcBounds(const char* text)
                {
                Bounds b;
                display.getTextBounds(text, 0, 0, &b.x, &b.y, &b.w, &b.h);
                return b;
                }
Center calcCenter(int x, int y){
  Center c;
  c.x = (SCREEN_WIDTH - x) / 2;
  c.y = (SCREEN_HEIGHT - y) / 2;
  return c;
}

void printCenter(const char* buf){
  Bounds b = calcBounds(buf);
  Center c = calcCenter(b.w, b.h);
  display.setCursor(c.x,c.y);
  display.println(buf);
}


void drawLoading(const char* c){
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  printCenter(c);
  display.display();
}

void drawTime(){
    int x = SCREEN_WIDTH-30;
    int y = 9;
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    char buf[6];

    if(!syncedOnce){
        strcpy(buf, "00:00");}
    else{
        getCurrentTimeHHMM(buf, sizeof(buf));}

    display.setCursor(x, y);
    //display.fillRect(x, y, 30, 8, SSD1306_BLACK);

    
    
    display.print(buf);
}