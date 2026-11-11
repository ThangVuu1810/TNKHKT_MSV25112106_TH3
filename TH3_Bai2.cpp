#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>

#define SV_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CH_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

BLECharacteristic* pChar;

class MyCallbacks : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic* pCharacteristic) {
    String rxValue = pCharacteristic->getValue();
    if (rxValue.length() > 0) {
      Serial.print(">nhan_ble:");
      Serial.println(rxValue.c_str());
    }
  }
};

void setup() {
  Serial.begin(115200);
  BLEDevice::init("BLE_NHOM10_25112031");

  auto* sv = BLEDevice::createServer();
  auto* se = sv->createService(SV_UUID);

  pChar = se->createCharacteristic(
    CH_UUID,
    BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE
  );

  pChar->setCallbacks(new MyCallbacks());
  pChar->setValue("MSSV: 25112031");
  se->start();

  BLEDevice::getAdvertising()->start();
}

void loop() {
  delay(2000);
}
