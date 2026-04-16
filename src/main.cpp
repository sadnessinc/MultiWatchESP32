#include <Arduino.h>
#include <Wire.h>
#include <WiFi.h>

#include "time.h"


#include "config/Pins.h"

#include "services/WiFi/instance/instanceServiceWiFi.h"
#include "core/AppState/AppState.h"
#include "core/Input/Input.h"

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "DHTesp.h"

Adafruit_SSD1306 display(128, 64, &Wire, -1);
InputService input;
DHTesp dht;

//WIFI
const char* ssid = "DOMRU_8FD2";
const char* password = "97698210";

//SCREENS
bool lastBtnState = HIGH;

//DEBOUNCE
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 150;

//SCREEN UPDATE
unsigned long lastScreenUpdate = 0;
const unsigned long screenInterval = 500;
bool needUpdateScreen = true;

void screenBTNTest();

//Button test
struct ButtonTestState
{
  bool up;
  bool down;
  bool left;
  bool right;
  bool ok;
  bool back;
};
ButtonTestState buttonTest;

//screen update
void checkScreenUpdate(){
  if (millis()-lastScreenUpdate >= screenInterval){
    lastScreenUpdate = millis();
    needUpdateScreen = true;
  }
}

void updateButtonTestInput();

void loop() {
  updateScreen();
  updateInput();
  updateSound();
}



void setup() {
  wifiService.begin(ssid, password);

  Wire.begin(21, 22);  // SDA=21, SCL=22
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.display();
  Serial.begin(115200);

  pinMode(BUZZER_PIN, OUTPUT);

  pinMode(BTN_UP, INPUT_PULLUP);
  pinMode(BTN_DOWN, INPUT_PULLUP);
  pinMode(BTN_LEFT, INPUT_PULLUP);
  pinMode(BTN_RIGHT, INPUT_PULLUP);
  pinMode(BTN_OK, INPUT_PULLUP);
  pinMode(BTN_BACK, INPUT_PULLUP);

  dht.setup(DHT_PIN, DHTesp::DHT11);
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
  display.print("BACK: ");    
  display.println(buttonTest.back ? "Pressed" : "Idle");
  display.display();
}

void updateButtonTestInput(){ 
  buttonTest.up = (digitalRead(BTN_UP) == LOW);
  buttonTest.down = (digitalRead(BTN_DOWN) == LOW);
  buttonTest.left = (digitalRead(BTN_LEFT) == LOW);
  buttonTest.right = (digitalRead(BTN_RIGHT) == LOW);
  buttonTest.ok = (digitalRead(BTN_OK) == LOW);
  buttonTest.back = (digitalRead(BTN_BACK) == LOW);
}
/*
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

*/
/*
*/

///
///LOGIC
///

/*void updateTimeInput(){
  if (handleButton(BTN_BACK))
  { currentScreen = 255; 
    return;}
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
}*/


