#include "AdditionalLib.h"

#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <Arduino.h>
#include "config/Pins.h"

#include "services\WiFi\instance\instanceServiceWiFi.h"

#include "core/DateTime/DateTime.h"

extern Adafruit_SSD1306 display;


extern bool syncedOnce;
extern bool lastSyncTimeChanged;
extern unsigned long lastSync;
extern tm lastSyncTime;

void drawWiFiIcon() {
  // Размер и позиция
  int x = SCREEN_WIDTH - 8;
  int y = 0;

  if (wifiService.isConnected()) {
    // Нарисуем простую "волнистую" иконку Wi-Fi
    display.drawCircle(x+3, y+3, 1, SSD1306_WHITE);
    display.drawCircle(x+3, y+3, 3, SSD1306_WHITE);
    display.drawCircle(x+3, y+3, 5, SSD1306_WHITE);
    display.fillCircle(x+3, y+3, 1, SSD1306_WHITE); // точка внизу
  } else {
    // Если не подключено — можно стереть иконку
    display.fillRect(x, y, 7, 7, SSD1306_BLACK);
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