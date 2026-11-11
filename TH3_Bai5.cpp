#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

#define LED 26

const char* ssid = "Dungxnguyenzzzz";
const char* pass = "hoilamgi";

const char* broker = "test.mosquitto.org";
const char* topicPub = "bcse/th3/nhom10/data";
const char* topicSub = "bcse/th3/nhom10/led";

WiFiClient wifiClient;
PubSubClient mqtt(wifiClient);

int dem = 0;
unsigned long lastPub = 0;

void callback(char* topic, byte* payload, unsigned int length) {
  char cmd = (char)payload[0];
  if (cmd == '1') {
    digitalWrite(LED, HIGH);
  } else if (cmd == '0') {
    digitalWrite(LED, LOW);
  }
}

void ketNoiMqtt() {
  while (!mqtt.connected()) {
    String clientId = "ESP32_Nhom10_" + String(random(0xffff), HEX);
    if (mqtt.connect(clientId.c_str())) {
      mqtt.subscribe(topicSub);
    } else {
      delay(2000);
    }
  }
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

  mqtt.setServer(broker, 1883);
  mqtt.setCallback(callback);
}

void loop() {
  if (!mqtt.connected()) {
    ketNoiMqtt();
  }
  mqtt.loop();

  if (millis() - lastPub >= 2000) {
    lastPub = millis();
    char msg[10];
    sprintf(msg, "%d", dem++);
    mqtt.publish(topicPub, msg);
    Serial.printf(">pub: %s\n", msg);
  }
}
