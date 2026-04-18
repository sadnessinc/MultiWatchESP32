#pragma once

#include <Arduino.h>

#pragma once

#include <Arduino.h>

class WiFiWebServer {
public:
    static void begin();
    static void update();

    static float temperature;
    static float humidity;

private:
    static void setupServer();
    static void setupWebSocket();
};