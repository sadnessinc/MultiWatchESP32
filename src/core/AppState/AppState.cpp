#include "AppState.h"

#include "screens/Menu/screenMenu.h"
#include "screens/WiFi/screenWiFi.h"
#include "screens/Amogus/screenAmogus.h"
#include "screens/HT/HT.h" 

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
        
        
    }
}

void updateSound(){
    if(currentScreen == AMOGUS){
        update_rtttl();}
    else{
        stop_rtttl();
    }
}