#pragma once

#include <Arduino.h>
#include "config/Pins.h"


class InputService{
    public:
    bool pressed(uint8_t pin);
    private:
    unsigned long lastPress = 0;
};