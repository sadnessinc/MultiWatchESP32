#include "screenMenu.h"

#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <Arduino.h>

#include "core/AppState/AppState.h"
#include "core/Input/Input.h"
#include "additional/AdditionalLib.h"

extern Adafruit_SSD1306 display;

extern InputService input;

//MENU
int start;
int visibleCount;
uint8_t menuIndex = 0;


const uint8_t menuCount = sizeof(menu) / sizeof(menu[0]);
int menuPage = 0;
const int itemsPerPage = 5;
int menuPagesCount = (menuCount + itemsPerPage - 1) / itemsPerPage;

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
    display.println(menu[index].name);
  }
  display.display();
}

void updateMenuInput(){
    calcMenuState();
    if (input.pressed(BTN_UP)){
      if (menuIndex == 0) menuIndex = visibleCount - 1;
      else menuIndex--;
      }
    if (input.pressed(BTN_DOWN)){
      if (menuIndex == visibleCount - 1) menuIndex = 0;
      else menuIndex++;
      }
    if (input.pressed(BTN_OK)){
      //needUpdateScreen = true;
      setScreen(menu[start+menuIndex].screen);

      }
    if (input.pressed(BTN_LEFT)){
      if (menuPage == 0) menuPage = menuPagesCount - 1;
      else menuPage--;
      menuIndex = 0;
      }
    if (input.pressed(BTN_RIGHT)){
      if (menuPage == menuPagesCount-1) menuPage = 0;
      else menuPage++;
      menuIndex = 0;
      }
}

void calcMenuState(){
  start = menuPage * itemsPerPage;
  visibleCount = menuCount - start;
  if (visibleCount > itemsPerPage) visibleCount = itemsPerPage;
}