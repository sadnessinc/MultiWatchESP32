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



void drawWiFiIcon() {
  // Размер и позиция
  int x = SCREEN_WIDTH - 8; //left time
  int y = 0;


const unsigned char wifiIcon[] PROGMEM = {
  0b00011000,
  0b00100100,
  0b01000010,
  0b00011000,
  0b00100100,
  0b00000000,
  0b00011000,
  0b00011000
};

    
  if (wifiNetwork.isConnected()) {
    display.drawBitmap(x, y, wifiIcon, 8, 8, SSD1306_WHITE);
  } else {
    // Если не подключено — можно стереть иконку
    display.fillRect(x, y, 8, 8, SSD1306_BLACK);
  }
}

void drawLastSync(){
    if(!syncedOnce){
      int16_t x1, y1;
      uint16_t w, h;
      display.setTextSize(1);         // маленький шрифт
      display.getTextBounds("Time don't change", 0, 0, &x1, &y1, &w, &h);
      int x = 2;  // небольшой отступ слева
      int y = 0;                     // верхний край

      // Стираем предыдущую область
      display.fillRect(x, y, w, h, SSD1306_BLACK);

      // Печатаем
      
      display.setCursor(x, y);
      display.setTextColor(SSD1306_WHITE);
      display.println("Time don't change");
      return;
    }

    char buf[20];
    snprintf(buf, sizeof(buf), "Last sync: %02d:%02d", lastSyncTime.tm_hour, lastSyncTime.tm_min);

    // Вычисляем позицию справа вверху
    int16_t x1, y1;
    uint16_t w, h;
    display.setTextSize(1);         // маленький шрифт
    display.getTextBounds(buf, 0, 0, &x1, &y1, &w, &h);
    int x = 2;  // небольшой отступ слева
    int y = 0;                     // верхний край

    // Стираем предыдущую область
    display.fillRect(x, y, w, h, SSD1306_BLACK);

    // Печатаем
    display.setCursor(x, y);
    display.setTextColor(SSD1306_WHITE);
    display.println(buf);
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
    display.fillRect(x, y, 30, 8, SSD1306_BLACK);

    
    
    display.print(buf);
}