#include "WiFiNetwork.h"

WiFiNetwork wifiNetwork;

WiFiNetwork::WiFiNetwork()
    : _mode(WiFiMode::OFF),
      _ssid(nullptr),
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

void WiFiNetwork::applyMode() {
    WiFi.disconnect(true);
    delay(50);

    switch (_mode) {
        case WiFiMode::OFF:    WiFi.mode(WIFI_OFF); break;
        case WiFiMode::STA:    WiFi.mode(WIFI_STA); break;
        case WiFiMode::AP:     WiFi.mode(WIFI_AP); break;
        case WiFiMode::AP_STA: WiFi.mode(WIFI_AP_STA); break;
    }

    _connecting = false;
    _connected = (WiFi.status() == WL_CONNECTED);
}

void WiFiNetwork::changeWiFiMode(WiFiMode mode) {
    _mode = mode;
    applyMode();
}

bool WiFiNetwork::connect(uint32_t timeoutMs) {
    if (!hasCredentials()) return false;
    if (_mode == WiFiMode::OFF) return false;
    if (_mode == WiFiMode::AP) return false;
    if (_connecting || isConnected()) return true;

    _connectTimeoutMs = timeoutMs;
    _connectStartMs = millis();
    _connecting = true;
    _connected = false;
    _failed = false;

    WiFi.begin(_ssid, _password);
    return true;
}

void WiFiNetwork::update() {
    if (_connecting) {
        if (WiFi.status() == WL_CONNECTED) {
            _connecting = false;
            _connected = true;
            _failed = false;
            _lastActivityMs = millis();
            return;
        }

        if (millis() - _connectStartMs >= _connectTimeoutMs) {
            stopConnection();
            _failed = true;
        }

        return;
    }

    if (_autoOffEnabled && isConnected()) {
        if (millis() - _lastActivityMs >= _autoOffMs) {
            disconnect();
        }
    }
}

void WiFiNetwork::stopConnection() {
    WiFi.disconnect(true);
    _connecting = false;
    _connected = false;
}

void WiFiNetwork::disconnect() {
    stopConnection();
    _mode = WiFiMode::OFF;
    WiFi.mode(WIFI_OFF);
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

WiFiNetwork::WiFiMode WiFiNetwork::getCurrentEnumWiFiMode() const {
    return _mode;
}

WiFiMode_t WiFiNetwork::getCurrentWiFiMode() const {
    return WiFi.getMode();
}

const char* WiFiNetwork::toStringWiFiMode(WiFiMode mode) const {
    switch (mode) {
        case WiFiMode::OFF:    return "OFF";
        case WiFiMode::STA:    return "STA";
        case WiFiMode::AP:     return "AP";
        case WiFiMode::AP_STA: return "AP_STA";
    }
    return "UNKNOWN";
}

void WiFiNetwork::prevWiFiMode() {
    switch (_mode) {
        case WiFiMode::OFF:    _mode = WiFiMode::AP_STA; break;
        case WiFiMode::STA:    _mode = WiFiMode::OFF;    break;
        case WiFiMode::AP:     _mode = WiFiMode::STA;    break;
        case WiFiMode::AP_STA: _mode = WiFiMode::AP;     break;
    }
    applyMode();
}

void WiFiNetwork::nextWiFiMode() {
    switch (_mode) {
        case WiFiMode::OFF:    _mode = WiFiMode::STA;    break;
        case WiFiMode::STA:    _mode = WiFiMode::AP;     break;
        case WiFiMode::AP:     _mode = WiFiMode::AP_STA; break;
        case WiFiMode::AP_STA: _mode = WiFiMode::OFF;    break;
    }
    applyMode();
}

bool WiFiNetwork::isAPMode(WiFiNetwork::WiFiMode mode) {
    return mode == WiFiNetwork::WiFiMode::AP ||
           mode == WiFiNetwork::WiFiMode::AP_STA;
}