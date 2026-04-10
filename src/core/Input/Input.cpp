#include "Input.h"

unsigned long debounceDelay = 150;

bool InputService::pressed(uint8_t pin){
    if(millis() - lastPress < debounceDelay)return false;
    if(digitalRead(pin) == LOW){
        lastPress = millis();
        return true;
    }
    return false;
}

