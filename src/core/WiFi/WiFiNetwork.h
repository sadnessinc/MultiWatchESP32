#pragma once

#include <Arduino.h>
#include <WiFi.h>

class WiFiNetwork {
public:
    enum class WiFiMode {
        OFF,
        STA,
        AP,
        AP_STA
    };

    WiFiNetwork();

    void setCredentials(const char* ssid, const char* password);

    bool connect(uint32_t timeoutMs = 5000);
    void update();
    void disconnect();      // полностью выключить Wi-Fi
    void stopConnection();  // только разорвать STA-соединение

    bool isConnected() const;
    bool isConnecting() const;
    bool connectFailed() const;
    bool hasCredentials() const;

    void setAutoOff(uint32_t ms = 30000);
    void keepAlive();

    void changeWiFiMode(WiFiMode mode);
    WiFiMode getCurrentEnumWiFiMode() const;
    WiFiMode_t getCurrentWiFiMode() const;
    const char* toStringWiFiMode(WiFiMode mode) const;

    void prevWiFiMode();
    void nextWiFiMode();

    bool isAPMode(WiFiNetwork::WiFiMode mode);

private:
    void applyMode();

    WiFiMode _mode;

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