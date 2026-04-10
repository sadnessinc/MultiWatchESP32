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

void setScreen(Screen s);
void updateInput();
void updateScreen();

extern Screen currentScreen;