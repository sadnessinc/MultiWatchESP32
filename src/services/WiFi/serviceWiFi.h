#pragma once
#include <Arduino.h>
#include <WiFi.h>

class WiFiService {
    public:
        void begin(const char* ssid, const char* password);
        bool connect(uint32_t timeout_ms = 10000);
        void disconnect();
        bool isConnected();

    private:
        const char* _ssid;
        const char* _password;
};
