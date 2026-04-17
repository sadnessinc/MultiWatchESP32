#include "screenHT.h"

#include "core/DHT/DHT.h"

#include "core/AppState/AppState.h"
#include "core/Input/Input.h"
#include "additional/AdditionalLib.h"

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHTesp.h>

extern Adafruit_SSD1306 display;
extern InputService input;

extern TempAndHumidity dataDHT;

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

void updateHTInput(){
  if (input.pressed(BTN_BACK))
  { setScreen(MENU); 
    return;}
  getDHTdata();
}