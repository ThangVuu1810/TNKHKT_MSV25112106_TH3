#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

#define LED 26

const char* ssid = "Dungxnguyenzzzz";
const char* pass = "hoilamgi";

WebServer server(80);
bool ledState = false;

void handleRoot() {
  String h = "<!DOCTYPE html><html><head><meta charset='utf-8'>";
  h += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  h += "<title>ESP32 LED Control</title></head><body style='font-family:sans-serif;text-align:center;padding-top:40px;'>";
  h += "<h2>BCSE - DIEU KHIEN LED QUA HTTP</h2>";
  h += "<p style='font-size:18px;'>Trang thai: <b style='color:" + String(ledState ? "green" : "red") + ";'>" + String(ledState ? "DANG BAT (ON)" : "DANG TAT (OFF)") + "</b></p>";
  h += "<div style='margin-top:20px;'>";
  h += "<a href='/on'><button style='padding:12px 24px;font-size:16px;background:green;color:white;border:none;border-radius:5px;margin:5px;'>BAT LED</button></a>";
  h += "<a href='/off'><button style='padding:12px 24px;font-size:16px;background:red;color:white;border:none;border-radius:5px;margin:5px;'>TAT LED</button></a>";
  h += "<a href='/toggle'><button style='padding:12px 24px;font-size:16px;background:#333;color:white;border:none;border-radius:5px;margin:5px;'>TOGGLE</button></a>";
  h += "</div></body></html>";
  server.send(200, "text/html", h);
}

void handleOn() {
  ledState = true;
  digitalWrite(LED, HIGH);
  server.sendHeader("Location", "/");
  server.send(303);
}

void handleOff() {
  ledState = false;
  digitalWrite(LED, LOW);
  server.sendHeader("Location", "/");
  server.send(303);
}

void handleToggle() {
  ledState = !ledState;
  digitalWrite(LED, ledState ? HIGH : LOW);
  server.sendHeader("Location", "/");
  server.send(303);
}

void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);

  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\n--- DA KET NOI WIFI ---");
  Serial.print("Truy cap Web Server tai: http://");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/on", handleOn);
  server.on("/off", handleOff);
  server.on("/toggle", handleToggle);
  server.begin();
}

void loop() {
  server.handleClient();
}
