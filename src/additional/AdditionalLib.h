#pragma once

#include "Arduino.h"

struct Bounds
{
  int16_t x, y;
  uint16_t w, h;
};
struct Center
{
  int x, y;
};

void drawWiFiIcon();
void drawLastSync();
void drawTime();


Bounds calcBounds(const char* text);
Center calcCenter(int x, int y);
void printCenter(const char* buf);
void drawLoading(const char* c);