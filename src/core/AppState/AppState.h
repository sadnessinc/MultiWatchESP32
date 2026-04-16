#pragma once

enum Screen {
    MENU,
    TIME,
    HT,
    CALENDAR,
    PLAYER,
    RADIO,
    WIFI,
    AMOGUS,
    BUTTONTEST
};

struct MenuItem {
    Screen screen;
    const char* name;
};

const MenuItem menu[]={
    {TIME, "//Time"},
    {HT, "Humidity/Temp"},
    {CALENDAR, "//Calendar"},
    {PLAYER, "//Player"},
    {RADIO, "//Radio"},
    {WIFI, "WiFi"},
    {AMOGUS, "Amogus"},
    {BUTTONTEST, "//Button test"} 
};
// //->work in progress (unworked)

void setScreen(Screen s);
void updateInput();
void updateScreen();
void updateSound();

extern Screen currentScreen;