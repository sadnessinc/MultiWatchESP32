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


void drawWiFiIcon();
void drawWiFiApIcon();

void drawWiFi(){
  drawWiFiIcon();
  drawWiFiApIcon();
}

void drawWiFiIcon() {
  // Размер и позиция
  int x = SCREEN_WIDTH - 8; //left time
  int y = 0;
  if (wifiNetwork.isConnected()){
  display.drawBitmap(x, y, wifiIcon, 8, 8, SSD1306_WHITE);
  } else {
    display.fillRect(x, y, 8, 8, SSD1306_BLACK);
  }
}

void drawWiFiApIcon(){
  int x = SCREEN_WIDTH - 17; //left wifi
  int y = 0;
  if (wifiNetwork.isAPMode(wifiNetwork.getCurrentEnumWiFiMode())){
  display.drawBitmap(x, y, WiFiApIcon, 8, 8, SSD1306_WHITE);
  } else {
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