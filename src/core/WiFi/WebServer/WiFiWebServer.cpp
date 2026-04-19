#include "WiFiWebServer.h"

#include <WiFi.h>
#include <WebServer.h>
#include <WebSocketsServer.h>

const char* AP_SSID = "ESP32_WWS";
const char* AP_PASS = "12345678";

WebServer server(80);
WebSocketsServer webSocket(81);

float WiFiWebServer::temperature = 0.0f;
float WiFiWebServer::humidity = 0.0f;

static void handleRoot() {
    String html =
        "<!doctype html><html><head><meta charset='utf-8'>"
        "<meta name='viewport' content='width=device-width, initial-scale=1'>"
        "<title>sadnessinc ESP32</title>"
        "<style>"
        "body{font-family:Arial,sans-serif;background:#111;color:#eee;padding:20px;}"
        ".card{max-width:320px;padding:16px;border:1px solid #333;border-radius:12px;}"
        "h1{font-size:22px;margin:0 0 12px 0;}"
        "h2{font-size:18px;margin:8px 0;}"
        "</style>"
        "</head><body>"
        "<div class='card'>"
        "<h1>sadnessinc ESP32</h1>"
        "<h2>Temp: <span id='t'>--</span> C</h2>"
        "<h2>Humidity: <span id='h'>--</span> %</h2>"
        "</div>"
        "<script>"
        "let ws = new WebSocket('ws://' + window.location.hostname + ':81/');"
        "ws.onmessage = function(e) {"
        "  try {"
        "    let d = JSON.parse(e.data);"
        "    document.getElementById('t').textContent = d.t.toFixed(1);"
        "    document.getElementById('h').textContent = d.h.toFixed(1);"
        "  } catch(err) {}"
        "};"
        "ws.onclose = function() {"
        "  document.getElementById('t').textContent = 'WS closed';"
        "};"
        "</script>"
        "</body></html>";

    server.send(200, "text/html; charset=utf-8", html);
}

static void webSocketEvent(uint8_t num, WStype_t type, uint8_t* payload, size_t length) {
    (void)num;
    (void)payload;
    (void)length;

    if (type == WStype_CONNECTED) {
        String json =
            "{\"t\":" + String(WiFiWebServer::temperature, 1) +
            ",\"h\":" + String(WiFiWebServer::humidity, 1) + "}";

        webSocket.sendTXT(num, json);
    }
}

void WiFiWebServer::setupServer() {
    server.on("/", handleRoot);
    server.begin();
}

void WiFiWebServer::setupWebSocket() {
    webSocket.begin();
    webSocket.onEvent(webSocketEvent);
}

void WiFiWebServer::begin() {
    //WiFi.mode(WIFI_AP);
    WiFi.softAP(AP_SSID, AP_PASS);

    setupServer();
    setupWebSocket();

    Serial.print("AP IP: ");
    Serial.println(WiFi.softAPIP());
}

void WiFiWebServer::update() {
    server.handleClient();
    webSocket.loop();

    static uint32_t lastPush = 0;
    if (millis() - lastPush >= 1000) {
        lastPush = millis();

        String json =
            "{\"t\":" + String(temperature, 1) +
            ",\"h\":" + String(humidity, 1) + "}";

        webSocket.broadcastTXT(json);
    }
}