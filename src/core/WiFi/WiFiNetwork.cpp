#include "WiFiNetwork.h"

WiFiNetwork wifiNetwork;

WiFiNetwork::WiFiNetwork()
    : _ssid(nullptr),
      _password(nullptr),
      _connecting(false),
      _connected(false),
      _failed(false),
      _connectTimeoutMs(5000),
      _connectStartMs(0) {}

void WiFiNetwork::setCredentials(const char* ssid, const char* password) {
    _ssid = ssid;
    _password = password;
}

bool WiFiNetwork::hasCredentials() const {
    return _ssid != nullptr && _password != nullptr;
}

bool WiFiNetwork::connect(uint32_t timeoutMs) {
    if (!hasCredentials()) return false;

    if (_connecting || isConnected()) return true;

    _connectTimeoutMs = timeoutMs;
    _connectStartMs = millis();
    _connecting = true;
    _connected = false;
    _failed = false;

    WiFi.mode(WIFI_AP_STA);
    WiFi.begin(_ssid, _password);

    return true;
}

void WiFiNetwork::update() {

    if (_connecting) {

        if (WiFi.status() == WL_CONNECTED) {
            _connecting = false;
            _connected = true;
            _failed = false;
            _lastActivityMs = millis();   // старт отсчёта авто-выключения
            return;
        }

        if (millis() - _connectStartMs >= _connectTimeoutMs) {
            disconnect();
            _failed = true;
        }

        return;
    }

    // авто-выключение
    if (_autoOffEnabled && isConnected()) {
        if (millis() - _lastActivityMs >= _autoOffMs) {
            disconnect();
        }
    }
}

void WiFiNetwork::disconnect() {
    WiFi.disconnect(true);
    WiFi.mode(WIFI_OFF);

    _connecting = false;
    _connected = false;
}

bool WiFiNetwork::isConnected() const {
    return WiFi.status() == WL_CONNECTED;
}

bool WiFiNetwork::isConnecting() const {
    return _connecting;
}

bool WiFiNetwork::connectFailed() const {
    return _failed;
}

void WiFiNetwork::setAutoOff(uint32_t ms) {
    _autoOffMs = ms;
    _autoOffEnabled = (ms > 0);
    _lastActivityMs = millis();
}

void WiFiNetwork::keepAlive() {
    _lastActivityMs = millis();
}