#pragma once

#include <Arduino.h>
#include <WiFi.h>

class WiFiNetwork {
public:
    WiFiNetwork();

    void setCredentials(const char* ssid, const char* password);

    // Старт подключения. Не ждёт результат.
    bool connect(uint32_t timeoutMs = 5000);

    // Вызывать часто из loop() или из timeUpdate()
    void update();

    void disconnect();

    bool isConnected() const;
    bool isConnecting() const;
    bool connectFailed() const;
    bool hasCredentials() const;

    void setAutoOff(uint32_t ms = 30000);
    void keepAlive();

private:
    const char* _ssid;
    const char* _password;

    bool _connecting;
    bool _connected;
    bool _failed;

    uint32_t _connectTimeoutMs;
    unsigned long _connectStartMs;

    uint32_t _autoOffMs = 0;
    unsigned long _lastActivityMs = 0;
    bool _autoOffEnabled = false;
};

extern WiFiNetwork wifiNetwork;