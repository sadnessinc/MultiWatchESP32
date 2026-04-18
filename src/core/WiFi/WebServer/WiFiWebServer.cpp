#include "WiFiWebServer.h"

#include <WiFi.h>
#include <WebServer.h>

const char* AP_SSID = "ESP32_WWS";
const char* AP_PASS = "12345678";

WebServer server(80);

float WiFiWebServer::temperature = 0;
float WiFiWebServer::humidity = 0;

static String makePage() {
    String html =
        "<html><body>"
        "<h1>sadnessinc ESP32</h1>"
        "<h2>Temp: " + String(WiFiWebServer::temperature) + " C</h2>"
        "<h2>Humidity: " + String(WiFiWebServer::humidity) + " %</h2>"
        "</body></html>";

    return html;
}

void handleRoot() {
    server.send(200, "text/html", makePage());
}

void WiFiWebServer::setupServer() {
    server.on("/", handleRoot);
    server.begin();
}

void WiFiWebServer::begin() {
    WiFi.softAP(AP_SSID, AP_PASS);
    setupServer();
}

void WiFiWebServer::update() {
    server.handleClient();
}