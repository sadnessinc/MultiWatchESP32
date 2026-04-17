#include "AppState.h"

#include "screens/Menu/screenMenu.h"
#include "screens/WiFi/screenWiFi.h"
#include "screens/Amogus/screenAmogus.h"
#include "screens/HT/screenHT.h" 
#include "screens/Time/screenTime.h"
#include "screens/ButtonTest/ButtonTest.h"

#include "core/Rtttl/Rtttl.h"

Screen currentScreen = MENU;

void setScreen(Screen s){
    currentScreen = s;
}

void updateScreen(){
    switch (currentScreen)
    {
    case MENU:
        screenMenu();
        break;
    
    case WIFI:
        screenWiFi();
        break;
    
    case AMOGUS:
        screenAmogus();
        break;
    
    case HT:
        screenHT();
        break;
    
    case TIME:
        screenTime();
        break;

    case BUTTONTEST:
        screenButtonTest();
        break;

    }
}

void updateInput(){
    switch (currentScreen)
    {
    case MENU:
        updateMenuInput();
        break;

    case WIFI:
        updateWiFiInput();
        break;

    case AMOGUS:
        updateAmogusInput();
        break;    

    case HT:
        updateHTInput();
        break;
        
    case TIME:
        updateTimeInput();
        break;

    case BUTTONTEST:
        updateButtonTestInput();
        break;
        
    }
}

void updateSound(){
    if(currentScreen == AMOGUS){
        update_rtttl();}
    else{
        stop_rtttl();
    }
}