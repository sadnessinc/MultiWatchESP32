#pragma once

#include <stdint.h>

enum Screen {
  SCREEN_MENU,
  SCREEN_TIME,
  SCREEN_HT,
  SCREEN_WIFI,
  SCREEN_AMOGUS,
  SCREEN_BTN_TEST
};

void setScreen(Screen s);
Screen getScreen();

void updateScreen();
void updateInput();