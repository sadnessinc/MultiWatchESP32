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
    BUTTONTEST,
    SNAKE
};

struct MenuItem {
    Screen screen;
    const char* name;
};

const MenuItem menu[]={
    {TIME, "Time"},
    {HT, "Humidity/Temp"},
    {WIFI, "WiFi"},
    {AMOGUS, "Amogus"},
    {SNAKE, "Snake"},
    {BUTTONTEST, "Button test"},
    {CALENDAR, "//Calendar"},
    {PLAYER, "//Player"},
    {RADIO, "//Radio"}

    
};
// //->work in progress (unworked)

void setScreen(Screen s);
void updateInput();
void updateScreen();
void updateSound();

extern Screen currentScreen;