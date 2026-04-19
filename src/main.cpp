#include <Arduino.h>
#include <Wire.h>
#include <WiFi.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "DHTesp.h"

#include "config/Pins.h"
//#include "config/mainConfig.h" //if i got vpn on my pc/server or in my country change president (add telegram bot)

#include "core/WiFi/WiFiNetwork.h"
#include "core/AppState/AppState.h"
#include "core/Input/Input.h"
#include "core/DateTime/DateTime.h"
#include "additional/AdditionalLib.h"
#include "core/WiFi/WebServer/WiFiWebServer.h"
#include "core/DHT/DHT.h"

Adafruit_SSD1306 display(128, 64, &Wire, -1);
InputService input;
DHTesp dht;

//WIFI
const char* ssid = "DOMRU_8FD2";
const char* password = "97698210";

void loop() {
  getDHTdata();

  wifiNetwork.update();
  WiFiWebServer::update();

  bool condition = true;

  timeUpdate();
  updateScreen();
  updateInput();
  updateSound();
  
}

void setup() {
  wifiNetwork.setCredentials(ssid, password);
  wifiNetwork.setAutoOff(30000); // 30 секунд после последнего использования
  wifiNetwork.changeWiFiMode(WiFiNetwork::WiFiMode::AP_STA);

  WiFiWebServer::begin();
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
  drawLoading("sadnessinc");
  display.display();
  delay(500);
}



/*

// For testing pin connections

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
}*/