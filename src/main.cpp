#include <Arduino.h>
#include <Wire.h>
#include <WiFi.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "time.h"
#include "DHTesp.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDR 0x3C
#define BUZZER_PIN 25

//DHT
#define DHT_PIN 33
DHTesp dht;
unsigned long lastHTDSync = 0;
const unsigned long syncHTDInterval = 2000;
TempAndHumidity dataDHT;

#define BTN_UP 15
#define BTN_DOWN 4
#define BTN_LEFT 5
#define BTN_RIGHT 23
#define BTN_BACK 18
#define BTN_OK 19

#define OCTAVE_OFFSET 0





//WIFI
const char* ssid = "DOMRU_8FD2";
const char* password = "97698210";
bool wifiConnected = false;

//DATE-TIME SYNC
const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 3 * 3600; // МСК +3
struct tm lastSyncTime;
bool syncedOnce = false;
bool lastSyncTimeChanged = false;
unsigned long lastSync = 0;
const unsigned long syncInterval = 3600000; // раз в час
const int daylightOffset_sec = 0;

//SCREENS
uint8_t currentScreen = 0;
uint8_t lastScreen = 255;
bool lastBtnState = HIGH;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

//DEBOUNCE
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 150;

//SCREEN UPDATE
unsigned long lastScreenUpdate = 0;
const unsigned long screenInterval = 500;
bool needUpdateScreen = true;



//MENU
uint8_t menuIndex = 0;
const char* menuItems[] = {
  "Time",//0
  "Humidity/Temp",//1
  "Calendar",//2
  "Player",//3
  "Radio",//4
  "WiFi",//5
  "Amogus",//6
  "Button Test",//7
};
const uint8_t menuCount = sizeof(menuItems) / sizeof(menuItems[0]);
int menuPage = 0;
const int itemsPerPage = 5;
int menuPagesCount = (menuCount + itemsPerPage - 1) / itemsPerPage;


//declaration
void updateInput();

void screenTime();
void screenHT();
void screenRadio();
void screenCalendar();
void screenWiFi();
void screenBTNTest();
void screenMenu();



//Button test
struct ButtonTestState
{
  bool up;
  bool down;
  bool left;
  bool right;
  bool ok;
};
ButtonTestState buttonTest;

//MENU
int start;
int visibleCount;

///
///DRAW
///
//AMOGUS
static const uint8_t amogus[800] PROGMEM = {
  0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
  0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
  0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00001111, 0b11111111, 0b11111111, 0b11111111, 0b10000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
  0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111110, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
  0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000011, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11100000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
  0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00001111, 0b11111111, 0b01010101, 0b01010101, 0b01111111, 0b11111111, 0b11111100, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
  0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00111111, 0b11110101, 0b11011101, 0b11011101, 0b10101010, 0b11011111, 0b11111111, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
  0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b01111111, 0b11011101, 0b01101101, 0b01101101, 0b10111011, 0b01101101, 0b11111111, 0b10000000, 0b00000000, 0b00000000, 0b00000000,
  0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b11111111, 0b01101101, 0b10110111, 0b10110111, 0b11111111, 0b11111111, 0b11111111, 0b11100000, 0b00000000, 0b00000000, 0b00000000,
  0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000001, 0b11111101, 0b10110110, 0b11011111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11110000, 0b00000000, 0b00000000, 0b00000000,
  0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000001, 0b11111101, 0b10111011, 0b01111111, 0b11111111, 0b11110000, 0b00000001, 0b11111111, 0b11111110, 0b00000000, 0b00000000, 0b00000000,
  0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000011, 0b11111010, 0b11011011, 0b11111111, 0b10000000, 0b00000000, 0b00000000, 0b00000001, 0b11111111, 0b10000000, 0b00000000, 0b00000000,
  0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000011, 0b11111011, 0b01101101, 0b01111111, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00011111, 0b11000000, 0b00000000, 0b00000000,
  0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000111, 0b11110101, 0b10110111, 0b11111110, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000111, 0b11100000, 0b00000000, 0b00000000,
  0b00000000, 0b00000000, 0b00000000, 0b00111111, 0b11111111, 0b11110110, 0b11011010, 0b11111100, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000011, 0b11110000, 0b00000000, 0b00000000,
  0b00000000, 0b00000000, 0b00000001, 0b11111111, 0b11111111, 0b11101011, 0b01101110, 0b11111100, 0b00100000, 0b00000000, 0b00000000, 0b00000000, 0b00000011, 0b11110000, 0b00000000, 0b00000000,
  0b00000000, 0b00000000, 0b00000111, 0b11111111, 0b11111111, 0b11101010, 0b10110110, 0b11111110, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000011, 0b11110000, 0b00000000, 0b00000000,
  0b00000000, 0b00000000, 0b00000111, 0b11111110, 0b10101111, 0b11101011, 0b11011011, 0b01111111, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000111, 0b11110000, 0b00000000, 0b00000000,
  0b00000000, 0b00000000, 0b00001111, 0b11110110, 0b11101111, 0b11101001, 0b01101101, 0b11111111, 0b11010010, 0b00000000, 0b00000000, 0b00000000, 0b01111111, 0b11100000, 0b00000000, 0b00000000,
  0b00000000, 0b00000000, 0b00001111, 0b11111011, 0b01011111, 0b11101011, 0b10110110, 0b10111111, 0b11111000, 0b01000100, 0b00100000, 0b10001111, 0b11111111, 0b11000000, 0b00000000, 0b00000000,
  0b00000000, 0b00000000, 0b00001111, 0b11101010, 0b10101111, 0b11101010, 0b11011011, 0b10110111, 0b11111111, 0b11111111, 0b01111111, 0b11111111, 0b11111111, 0b10000000, 0b00000000, 0b00000000,
  0b00000000, 0b00000000, 0b00001111, 0b11101010, 0b10101111, 0b11010101, 0b01101101, 0b10111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b01111111, 0b00000000, 0b00000000, 0b00000000,
  0b00000000, 0b00000000, 0b00001111, 0b11101010, 0b10101111, 0b11101010, 0b10110110, 0b11010101, 0b01110111, 0b11111111, 0b11111110, 0b11101101, 0b11111111, 0b00000000, 0b00000000, 0b00000000,
  0b00000000, 0b00000000, 0b00001111, 0b11010101, 0b01011111, 0b11101010, 0b11011011, 0b01110111, 0b01011111, 0b10110110, 0b11011011, 0b01101110, 0b10111111, 0b00000000, 0b00000000, 0b00000000,
  0b00000000, 0b00000000, 0b00001111, 0b11010101, 0b01011111, 0b11010101, 0b01101101, 0b10110101, 0b11101010, 0b11011011, 0b01101101, 0b10110110, 0b11111111, 0b10000000, 0b00000000, 0b00000000,
  0b00000000, 0b00000000, 0b00001111, 0b11010101, 0b01001111, 0b11101010, 0b10110110, 0b11011110, 0b10111011, 0b01101101, 0b10110110, 0b11011011, 0b01111111, 0b10000000, 0b00000000, 0b00000000,
  0b00000000, 0b00000000, 0b00001111, 0b11010101, 0b01011111, 0b11010101, 0b01011011, 0b01101011, 0b10101101, 0b10110110, 0b11011011, 0b01101101, 0b01111111, 0b10000000, 0b00000000, 0b00000000,
  0b00000000, 0b00000000, 0b00001111, 0b11010101, 0b01011111, 0b11101010, 0b10101101, 0b10111010, 0b11110110, 0b11011011, 0b01101101, 0b10110110, 0b10111111, 0b10000000, 0b00000000, 0b00000000,
  0b00000000, 0b00000000, 0b00001111, 0b11010101, 0b01001111, 0b11101010, 0b10101010, 0b11011011, 0b01011011, 0b01101101, 0b10110110, 0b11011010, 0b10111111, 0b10000000, 0b00000000, 0b00000000,
  0b00000000, 0b00000000, 0b00001111, 0b11010101, 0b01011111, 0b11101010, 0b10101011, 0b01101101, 0b11011101, 0b10110110, 0b11011011, 0b01101010, 0b11111111, 0b10000000, 0b00000000, 0b00000000,
  0b00000000, 0b00000000, 0b00001111, 0b11010101, 0b01001111, 0b11101010, 0b10101010, 0b10110110, 0b11010110, 0b11011011, 0b01101101, 0b10110101, 0b01111111, 0b00000000, 0b00000000, 0b00000000,
  0b00000000, 0b00000000, 0b00001111, 0b11101010, 0b10101111, 0b11101010, 0b10101010, 0b10101011, 0b01111011, 0b01101101, 0b10110110, 0b10101010, 0b11111111, 0b00000000, 0b00000000, 0b00000000,
  0b00000000, 0b00000000, 0b00001111, 0b11101010, 0b10101111, 0b11101010, 0b10101010, 0b10101010, 0b10001010, 0b10101010, 0b10101010, 0b10101010, 0b11111111, 0b00000000, 0b00000000, 0b00000000,
  0b00000000, 0b00000000, 0b00000111, 0b11101010, 0b10101111, 0b11101010, 0b10101010, 0b10101010, 0b10101010, 0b10101010, 0b10101010, 0b10101010, 0b11111110, 0b00000000, 0b00000000, 0b00000000,
  0b00000000, 0b00000000, 0b00000111, 0b11111010, 0b10101111, 0b11101010, 0b10101010, 0b10101010, 0b10101010, 0b10101010, 0b10101010, 0b10101010, 0b11111110, 0b00000000, 0b00000000, 0b00000000,
  0b00000000, 0b00000000, 0b00000011, 0b11111111, 0b11111111, 0b11101010, 0b10101010, 0b10101010, 0b11111010, 0b10101010, 0b10101010, 0b10101011, 0b11111110, 0b00000000, 0b00000000, 0b00000000,
  0b00000000, 0b00000000, 0b00000000, 0b11111111, 0b11111111, 0b11101010, 0b10101010, 0b10101011, 0b11111111, 0b11111111, 0b11111110, 0b10101011, 0b11111100, 0b00000000, 0b00000000, 0b00000000,
  0b00000000, 0b00000000, 0b00000000, 0b00001111, 0b11111111, 0b11110101, 0b01010101, 0b01010111, 0b11111111, 0b11111111, 0b11101010, 0b10101011, 0b11111100, 0b00000000, 0b00000000, 0b00000000,
  0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000111, 0b11110101, 0b01010101, 0b01010111, 0b11111100, 0b11111111, 0b01010101, 0b01010111, 0b11111000, 0b00000000, 0b00000000, 0b00000000,
  0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000111, 0b11110101, 0b01010101, 0b01010111, 0b11111000, 0b01111111, 0b01010101, 0b01010111, 0b11111000, 0b00000000, 0b00000000, 0b00000000,
  0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000011, 0b11110101, 0b01010101, 0b01010111, 0b11111000, 0b01111111, 0b01010101, 0b01010111, 0b11111000, 0b00000000, 0b00000000, 0b00000000,
  0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000011, 0b11111010, 0b10101010, 0b10101011, 0b11111000, 0b00111111, 0b01010101, 0b01010111, 0b11110000, 0b00000000, 0b00000000, 0b00000000,
  0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000011, 0b11111010, 0b10101010, 0b10101111, 0b11111110, 0b11111111, 0b11101010, 0b11011111, 0b11100000, 0b00000000, 0b00000000, 0b00000000,
  0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00001011, 0b11111101, 0b01010101, 0b01011111, 0b11110010, 0b10011111, 0b11111111, 0b11111111, 0b10000000, 0b00000000, 0b00000000, 0b00000000,
  0b00000000, 0b00000000, 0b00000000, 0b00000101, 0b01010101, 0b11111111, 0b11111111, 0b11111111, 0b11101010, 0b01010011, 0b11111111, 0b11111101, 0b01010101, 0b01000000, 0b00000000, 0b00000000,
  0b00000000, 0b00000000, 0b00000000, 0b00000010, 0b01010010, 0b11111111, 0b11111111, 0b11111111, 0b00101001, 0b01001010, 0b01000100, 0b01001010, 0b01010010, 0b01010000, 0b00000000, 0b00000000,
  0b00000000, 0b00000000, 0b00000000, 0b00000101, 0b01001010, 0b00100101, 0b11111111, 0b11110001, 0b01010101, 0b01010101, 0b00110011, 0b00101001, 0b01001010, 0b10000000, 0b00000000, 0b00000000,
  0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000111, 0b01010101, 0b00100100, 0b10010101, 0b01001010, 0b01010010, 0b10010100, 0b10100101, 0b01100000, 0b00000000, 0b00000000, 0b00000000,
  0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
  0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
};
void drawAmogus(){
  display.setTextColor(SSD1306_WHITE);
  // Для получения этого кода использовался онлайн конвертер https://duino.ru/media/image-converter/index.html 
  display.drawBitmap(0, 14, amogus, 128, 50, SSD1306_WHITE);
}

void drawWiFiIcon() {
  // Размер и позиция
  int x = SCREEN_WIDTH - 8;
  int y = 0;

  if (wifiConnected) {
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

void drawLoading(const char* c){
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  printCenter(c);
  display.display();
}


///
///ADDITIONAL FUNCTIONS
///

//printCenter

struct Bounds
{
  int16_t x, y;
  uint16_t w, h;
};
struct Center
{
  int x, y;
};
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

//buttons
bool handleButton(uint8_t btn){
  static unsigned long lastPress = 0;
  bool buttonState = digitalRead(btn);
  if (millis()- lastPress < debounceDelay)return false;
  if (buttonState == LOW){
    lastPress = millis();
    return true;
  }
  else{return false;}
}

//wifi
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

//menu
void calcMenuState(){
  start = menuPage * itemsPerPage;
  visibleCount = menuCount - start;
  if (visibleCount > itemsPerPage) visibleCount = itemsPerPage;
}

//screen update
void checkScreenUpdate(){
  if (millis()-lastScreenUpdate >= screenInterval){
    lastScreenUpdate = millis();
    needUpdateScreen = true;
  }
}

//dht
void getDHTdata(){
  unsigned long now = millis();
  if(now - lastHTDSync < syncHTDInterval)return;
  lastHTDSync = now;
  dataDHT = dht.getTempAndHumidity();
}

//rtttl
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
const int notes[] = { 0,
NOTE_C4, NOTE_CS4, NOTE_D4, NOTE_DS4, NOTE_E4, NOTE_F4, NOTE_FS4, NOTE_G4, NOTE_GS4, NOTE_A4, NOTE_AS4, NOTE_B4,
NOTE_C5, NOTE_CS5, NOTE_D5, NOTE_DS5, NOTE_E5, NOTE_F5, NOTE_FS5, NOTE_G5, NOTE_GS5, NOTE_A5, NOTE_AS5, NOTE_B5,
NOTE_C6, NOTE_CS6, NOTE_D6, NOTE_DS6, NOTE_E6, NOTE_F6, NOTE_FS6, NOTE_G6, NOTE_GS6, NOTE_A6, NOTE_AS6, NOTE_B6,
NOTE_C7, NOTE_CS7, NOTE_D7, NOTE_DS7, NOTE_E7, NOTE_F7, NOTE_FS7, NOTE_G7, NOTE_GS7, NOTE_A7, NOTE_AS7, NOTE_B7
};
const char *song = "Custom:d=8,o=5,b=120:c,8d#,8f,8f#,8f,8d#,8c,8p,8a#,8d,8c,8p,8p,8p,8p,c,8d#,8f,8f#,8f,8d#,8f#,8p,8f#,8f,8d#,8f#,8f,8d#";
bool playSound = false;
bool playOnce = true;

#define isdigit(n) (n >= '0' && n <= '9')

const char* rtttlPtr = nullptr;
unsigned long noteEnd = 0;
bool rtttlPlaying = false;
static byte default_dur = 4;
static byte default_oct = 6;
static int bpm = 63;
static long wholenote = 0;
static bool headerParsed = false;
void start_rtttl(const char* p) {
  rtttlPtr = p;

  while (*rtttlPtr && *rtttlPtr != ':') rtttlPtr++;
  if (*rtttlPtr == ':') rtttlPtr++;

  noteEnd = 0;
  rtttlPlaying = true;
  headerParsed = false;
}
void stop_rtttl(){
  if(!rtttlPlaying)return;
  rtttlPlaying = false;
  headerParsed = false;
  rtttlPtr = nullptr;
  noteEnd = 0;
  noTone(BUZZER_PIN);
}
void update_rtttl() {
  if (!rtttlPlaying || !rtttlPtr) return;

  unsigned long now = millis();
  if (noteEnd && now < noteEnd) return;

  if (!headerParsed) {
    default_dur = 4;
    default_oct = 6;
    bpm = 63;

    while (*rtttlPtr && *rtttlPtr != ':') {
      if (*rtttlPtr == 'd' && rtttlPtr[1] == '=') {
        rtttlPtr += 2;
        int num = 0;
        while (isdigit(*rtttlPtr)) num = num * 10 + (*rtttlPtr++ - '0');
        if (num > 0) default_dur = num;
      } 
      else if (*rtttlPtr == 'o' && rtttlPtr[1] == '=') {
        rtttlPtr += 2;
        if (isdigit(*rtttlPtr)) default_oct = *rtttlPtr++ - '0';
      } 
      else if (*rtttlPtr == 'b' && rtttlPtr[1] == '=') {
        rtttlPtr += 2;
        int num = 0;
        while (isdigit(*rtttlPtr)) num = num * 10 + (*rtttlPtr++ - '0');
        if (num > 0) bpm = num;
      } 
      else {
        rtttlPtr++;
      }

      if (*rtttlPtr == ',') rtttlPtr++;
    }

    if (*rtttlPtr != ':') {
      rtttlPlaying = false;
      noTone(BUZZER_PIN);
      return;
    }

    rtttlPtr++;
    wholenote = (60 * 1000L / bpm) * 4;
    headerParsed = true;
  }

  while (*rtttlPtr == ' ' || *rtttlPtr == ',') rtttlPtr++;

  if (*rtttlPtr == 0) {
    rtttlPlaying = false;
    noTone(BUZZER_PIN);
    return;
  }

  int num = 0;
  while (isdigit(*rtttlPtr)) num = num * 10 + (*rtttlPtr++ - '0');

  long duration = (num > 0) ? (wholenote / num) : (wholenote / default_dur);

  byte note = 0;
  switch (*rtttlPtr) {
    case 'c': note = 1;  break;
    case 'd': note = 3;  break;
    case 'e': note = 5;  break;
    case 'f': note = 6;  break;
    case 'g': note = 8;  break;
    case 'a': note = 10; break;
    case 'b': note = 12; break;
    case 'p': note = 0;  break;
  }
  if (*rtttlPtr) rtttlPtr++;

  if (*rtttlPtr == '#') {
    note++;
    rtttlPtr++;
  }

  if (*rtttlPtr == '.') {
    duration += duration / 2;
    rtttlPtr++;
  }

  byte scale = default_oct;
  if (isdigit(*rtttlPtr)) {
    scale = *rtttlPtr - '0';
    rtttlPtr++;
  }
  scale += OCTAVE_OFFSET;

  if (*rtttlPtr == ',') rtttlPtr++;

  if (note) {
    int idx = (scale - 4) * 12 + note;
    if (idx >= 0 && idx < (int)(sizeof(notes) / sizeof(notes[0]))) {
      tone(BUZZER_PIN, notes[idx]);
    } else {
      noTone(BUZZER_PIN);
    }
  } else {
    noTone(BUZZER_PIN);
  }

  noteEnd = now + duration;
}

//time
void checkLastSync(){
  unsigned long now = millis();
  // периодическая синхронизация раз в syncInterval
  if (now - lastSync > syncInterval) {
    if(!wifiConnected){
      connectWiFi();
    }
    if(wifiConnected){
      if(syncTime()){
        disconnectWiFi();
      }
    }
    
  }else if (wifiConnected)
  {
    disconnectWiFi();
  }
  
}

bool syncTime (bool needPrint = false, uint32_t timeoutMs = 5000){
  if (needPrint) drawLoading("Syncing");
  if (!wifiConnected) return false;
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  struct tm timeinfo;
  if (getLocalTime(&timeinfo, timeoutMs)) {
        Serial.println("Time synced via NTP");
        lastSyncTime = timeinfo;
        syncedOnce = true;
        lastSync = millis();
        lastSyncTimeChanged = true;
        Serial.printf("Time synced: %02d:%02d\n", timeinfo.tm_hour, timeinfo.tm_min);
        return true;
  }
  Serial.println("NTP sync failed");
  return false;
}


//
///BASE
//
void loop() {
  updateInput();
  //updateMenuInput();
  changeScreen();
  checkScreenUpdate();
  //checkLastSync(); //включить после подключения постоянного акб
  if(currentScreen == 1){
    update_rtttl();}
  else{
    stop_rtttl();
  }
}

void setup() {
  Wire.begin(21, 22);  // SDA=21, SCL=22
  Serial.begin(115200);
  pinMode(BUZZER_PIN, OUTPUT);

  pinMode(BTN_UP, INPUT_PULLUP);
  pinMode(BTN_DOWN, INPUT_PULLUP);
  pinMode(BTN_LEFT, INPUT_PULLUP);
  pinMode(BTN_RIGHT, INPUT_PULLUP);
  pinMode(BTN_OK, INPUT_PULLUP);
  pinMode(BTN_BACK, INPUT_PULLUP);

  dht.setup(DHT_PIN, DHTesp::DHT11);

  if(!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println("SSD1306 allocation failed");
    for(;;);
  }

  display.clearDisplay();
  currentScreen = 255;
}

///
///UI
///

void screenCalendar(){

}

void changeScreen(){
  switch (currentScreen)
  {
  case 0:
    screenTime();
    break;
  case 1:
    screenHT();
    break;
  case 2:
    break;
  case 3:
    break;
  case 4:
    break;
  case 5:
    screenWiFi();
    break;
  case 6:
    screenAmogus();
    break;
  case 7:
    screenBTNTest();
    break;
  case 255:
    screenMenu();
    break;
  }
}

void screenMenu(){
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0,0);
  display.setTextColor(SSD1306_WHITE);
  display.print("Menu");
  display.setTextSize(1);
  drawWiFiIcon();
  calcMenuState();

  for(int i = 0; i < visibleCount; i++){
    int index = start + i;
    if (index >= menuCount) break;
    int y = 17+i*10;
    if (i == menuIndex) {
      display.fillRect(0, y - 1, 128, 10, SSD1306_WHITE);
      display.setTextColor(SSD1306_BLACK);
    } else{
      display.setTextColor(SSD1306_WHITE);
    }
    display.setCursor(4,y);
    display.print(menuItems[index]);
  }
  display.display();
}

void screenBTNTest(){
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Button Test");
  display.setCursor(0,16);
  display.print("UP: ");    
  display.println(buttonTest.up ? "Pressed" : "Idle");
  display.print("DOWN: ");  
  display.println(buttonTest.down ? "Pressed" : "Idle");
  display.print("LEFT: ");  
  display.println(buttonTest.left ? "Pressed" : "Idle");
  display.print("RIGHT: "); 
  display.println(buttonTest.right ? "Pressed" : "Idle");
  display.print("OK: ");    
  display.println(buttonTest.ok ? "Pressed" : "Idle");
  display.display();
}

void screenWiFi(){
  if (!needUpdateScreen) return;
  needUpdateScreen = false;

  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0,0);
  display.setTextColor(SSD1306_WHITE);
  display.println("WiFi");

  if (wifiConnected){
    display.println("Status:ON");
    display.setTextSize(1);
    display.println("Press DOWN to disable");
  }
  else {
    display.println("Status:OFF");
    display.setTextSize(1);
    display.println("Press DOWN to enable");
  }
  display.display();
}

void screenTime(){
  drawLoading("Loading");
  display.display();
  struct tm timeinfo;
  if (getLocalTime(&timeinfo)) {
    display.clearDisplay();
    display.setTextSize(4);
    display.setTextColor(SSD1306_WHITE);
    char buf[6];
    sprintf(buf, "%02d:%02d", timeinfo.tm_hour, timeinfo.tm_min);
    printCenter(buf);
    drawWiFiIcon();   
    drawLastSync();
    display.display();
    return;
  }
  
  drawLoading("Need sync");

  if (wifiConnected){
    syncTime(true);
  } else if (connectWiFi(true)){
    syncTime(true);
  }
  else{
    Serial.println("No time info");
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    printCenter("Time failed");
    drawWiFiIcon();
    display.display();
  }
}

void screenAmogus(){
  if(!needUpdateScreen)return;
  needUpdateScreen = false;

  display.clearDisplay();
  drawAmogus();
  display.display();
}

void screenHT(){
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(2);
  Bounds b = calcBounds("TEMP/HUM");
  Center c = calcCenter(b.w,b.h);
  display.setCursor(c.x,0);
  display.print("TEMP/HUM");

  display.setTextSize(3);
  
  b = calcBounds("00 C");
  c = calcCenter(b.w,b.h);
  display.setCursor(c.x, 16);
  display.print(int(dataDHT.temperature));
  display.println(" C");

  display.setCursor(c.x, 40);
  display.print(int(dataDHT.humidity));
  display.println(" %");

  display.display();
}

///
///LOGIC
///

void updateTimeInput(){
  if (handleButton(BTN_BACK))
  { currentScreen = 255; 
    return;}
}

void updateHTInput(){
  if (handleButton(BTN_BACK))
  { currentScreen = 255; 
    return;}
  getDHTdata();
}

void updateCalendarInput(){
  if (handleButton(BTN_BACK))
  { currentScreen = 255; 
    return;}
}

void updatePlayerInput(){
  if (handleButton(BTN_BACK))
  { currentScreen = 255; 
    return;}
}

void updateRadioInput(){
  if (handleButton(BTN_BACK))
  { currentScreen = 255; 
    return;}
}

void updateWiFiInput(){
  if (handleButton(BTN_BACK))
  { currentScreen = 255; 
    return;}

  if (handleButton(BTN_DOWN)){
    if(!wifiConnected)connectWiFi();
    else disconnectWiFi();
    needUpdateScreen = true;
  }
}

void updateAmogusInput(){
  if (handleButton(BTN_BACK))
  { currentScreen = 255; 
    return;}
  bool btnDown = handleButton(BTN_DOWN);
  if(playOnce){
    playOnce = false;
    start_rtttl(song);
  }
  
  if (btnDown){
    playOnce = true;
  }
}

void updateButtonTestInput(){ 
  if (handleButton(BTN_BACK))
  { currentScreen = 255; 
    return;}
  
  buttonTest.up = (digitalRead(BTN_UP) == LOW);
  buttonTest.down = (digitalRead(BTN_DOWN) == LOW);
  buttonTest.left = (digitalRead(BTN_LEFT) == LOW);
  buttonTest.right = (digitalRead(BTN_RIGHT) == LOW);
  buttonTest.ok = (digitalRead(BTN_OK) == LOW);
}

void updateMenuInput(){
    calcMenuState();
    if (handleButton(BTN_UP)){
      if (menuIndex == 0) menuIndex = visibleCount - 1;
      else menuIndex--;
      }
    if (handleButton(BTN_DOWN)){
      if (menuIndex == visibleCount - 1) menuIndex = 0;
      else menuIndex++;
      }
    if (handleButton(BTN_OK)){
      needUpdateScreen = true;
      currentScreen = start + menuIndex;
      }
    if (handleButton(BTN_LEFT)){
      if (menuPage == 0) menuPage = menuPagesCount - 1;
      else menuPage--;
      menuIndex = 0;
      }
    if (handleButton(BTN_RIGHT)){
      if (menuPage == menuPagesCount-1) menuPage = 0;
      else menuPage++;
      menuIndex = 0;
      }
    }

void updateInput(){
  switch (currentScreen)
  {
  case 255:
    updateMenuInput();
    break;

    case 0:
    updateTimeInput();
    break;

    case 1:
    updateHTInput();
    break;

    case 2:
    updateCalendarInput();
    break;

    case 3:
    updatePlayerInput();
    break;

    case 4:
    updateRadioInput();
    break;

    case 5:
    updateWiFiInput();
    break;

    case 6:
    updateAmogusInput();
    break;

    case 7:
    updateButtonTestInput();
    break;
  }
}