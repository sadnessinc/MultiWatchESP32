#include "ButtonTest.h"

#include "core/AppState/AppState.h"
#include "core/Input/Input.h"

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

extern Adafruit_SSD1306 display;
extern InputService input;

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


void screenButtonTest(){
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
//  display.println(buttonTest.ok ? "Pressed" : "Idle");
//  display.print("BACK: ");    
  display.println(buttonTest.back ? "Pressed" : "Idle");
  display.display();
}

void updateButtonTestInput(){ 
  buttonTest.up = (digitalRead(BTN_UP) == LOW);
  buttonTest.down = (digitalRead(BTN_DOWN) == LOW);
  buttonTest.left = (digitalRead(BTN_LEFT) == LOW);
  buttonTest.right = (digitalRead(BTN_RIGHT) == LOW);
  buttonTest.ok = (digitalRead(BTN_OK) == LOW);
//  buttonTest.back = (digitalRead(BTN_BACK) == LOW);
  if(input.pressed(BTN_BACK)){
        setScreen(MENU);
    }
}