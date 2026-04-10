#include "AppState.h"

#include "screens/Menu/screenMenu.h"
#include "screens/WiFi/screenWiFi.h"

Screen currentScreen = MENU;

void setScreen(Screen s){
    currentScreen = s;
}

void updateScreen(){
    switch (currentScreen)
    {
    case MENU:
        //screenMenu();
        break;
    
    case WIFI:
        screenWiFi();
        break;
    }
}

void updateInput(){
    switch (currentScreen)
    {
    case MENU:
        //updateMenuInput();
        break;

    case WIFI:
        updateWiFiInput();
        break;
    }
}