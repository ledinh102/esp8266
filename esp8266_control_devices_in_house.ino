#include <Arduino.h>
#include "DHT.h"
#include "setup_project.h"
#include "write_data.h"
#include "listen_data_change.h"

#define GAS_SENSOR_PIN A0
#define GAS_SENSOR_THRESHOLD 500

#define DHTPIN 9  // SD2
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

unsigned long sendDataPrevMillis = 0;
unsigned long readDataPrevMillis = 0;

int count = 0;

void setup() {
  Serial.begin(115200);

  setupWifi();

  setupFirebase();
  pinMode(D0, OUTPUT);
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(D4, OUTPUT);
  pinMode(D6, OUTPUT);
  pinMode(D7, OUTPUT);
  pinMode(D8, OUTPUT);

  // addHouse("Nhà của Hiền", "tranminhhien@gmail.com", "0922942242", "lUvEAKn9qFv6PX2thgnFDQ==");
  // addHouse("Nhà của Hường", "lethihuong@gmail.com", "0984842341", "GjqxsM03+paMWHNroz7jjg==");

  listenChange();

  dht.begin();
}

void loop() {
  // if (millis() - readDataPrevMillis > 5 || readDataPrevMillis == 0) {
  //   readDataPrevMillis = millis();
    if (Firebase.getInt(fbdo, "/test1/e3ba4698-d64b-447e-81f5-0bf0e09700eb/floors/e72cf36f-f9c5-4dee-b11a-951c0e3dc638/rooms/4d71d15e-9dc8-4e39-b506-facef152130a/devices/72e2d23f-2602-4071-947f-38ee4fe5ef7e/state")) {
      int state = fbdo.intData();  // Get the float value
      digitalWrite(D0, state == 0 ? LOW : HIGH);
    } else {
      Serial.println("Failed to get temperature from Firebase");
    }
    if (Firebase.getInt(fbdo, "/test1/e3ba4698-d64b-447e-81f5-0bf0e09700eb/floors/e72cf36f-f9c5-4dee-b11a-951c0e3dc638/rooms/4e9473a3-3cd9-45b9-b33b-bf264cf43e04/devices/a76be91b-8087-4987-a94e-0419756cd748/state")) {
      int state = fbdo.intData();  // Get the float value
      digitalWrite(D1, state == 0 ? LOW : HIGH);
    } else {
      Serial.println("Failed to get temperature from Firebase");
    }
    if (Firebase.getInt(fbdo, "/test1/e3ba4698-d64b-447e-81f5-0bf0e09700eb/floors/e72cf36f-f9c5-4dee-b11a-951c0e3dc638/rooms/ae42c2e9-eb8c-4777-b89b-cce821206550/devices/494c63c9-1d6e-4535-94d7-c1145757807d/state")) {
      int state = fbdo.intData();  // Get the float value
      digitalWrite(D2, state == 0 ? LOW : HIGH);
    } else {
      Serial.println("Failed to get temperature from Firebase");
    }
    if (Firebase.getInt(fbdo, "/test1/e3ba4698-d64b-447e-81f5-0bf0e09700eb/floors/40b38a85-36ea-4a72-921a-2fbb579a2daf/rooms/2de1b963-b2c3-48bc-be79-81976b2b6ab9/devices/896c0b60-590d-4634-b132-8f5eab39044c/state")) {
      int state = fbdo.intData();  // Get the float value
      digitalWrite(D3, state == 0 ? LOW : HIGH);
    } else {
      Serial.println("Failed to get temperature from Firebase");
    }
    if (Firebase.getInt(fbdo, "/test1/e3ba4698-d64b-447e-81f5-0bf0e09700eb/floors/40b38a85-36ea-4a72-921a-2fbb579a2daf/rooms/3fdfefc2-8041-4416-848b-3c300891f9b3/devices/b9a23198-179b-42a8-b05e-58bded59c2c9/state")) {
      int state = fbdo.intData();  // Get the float value
      digitalWrite(D4, state == 0 ? LOW : HIGH);
    } else {
      Serial.println("Failed to get temperature from Firebase");
    }

    if (Firebase.getFloat(fbdo, "/test1/e3ba4698-d64b-447e-81f5-0bf0e09700eb/floors/e72cf36f-f9c5-4dee-b11a-951c0e3dc638/rooms/4d71d15e-9dc8-4e39-b506-facef152130a/DHT/1e1397cf-de32-4c6b-bc51-52b5ec8277ba/temperature")) {
      float temp = fbdo.floatData();  // Get the float value
      if (temp > 35 || analogRead(GAS_SENSOR_PIN) > 500) {
        tone(D5, 1000);
        Serial.println("> 35");
      } else {
        noTone(D5);
        Serial.println("< 35");
      }
    } else {
      Serial.println("Failed to get temperature from Firebase");
    }

    if (Firebase.getInt(fbdo, "/test1/e3ba4698-d64b-447e-81f5-0bf0e09700eb/floors/e72cf36f-f9c5-4dee-b11a-951c0e3dc638/rooms/4d71d15e-9dc8-4e39-b506-facef152130a/devices/12d2378c-6f75-49c2-94dc-831b8bb1e121/state")) {
      int state = fbdo.intData();  // Get the float value
      digitalWrite(D6, LOW);
      digitalWrite(D7, HIGH);
      if (state == 0) analogWrite(D8, 0);
      else if (state == 1) analogWrite(D8, 85);
      else if (state == 2) analogWrite(D8, 170);
      else if (state == 3) analogWrite(D8, 255);
    } else {
      Serial.println("Failed to get temperature from Firebase");
    }

    if (Firebase.ready() && (millis() - sendDataPrevMillis > 10000 || sendDataPrevMillis == 0)) {
      float h = dht.readHumidity();
      float t = dht.readTemperature();
      sendDataPrevMillis = millis();
      count++;
      FirebaseJson dhtJson;
      dhtJson.add("temperature", t);
      dhtJson.add("humidity", h);

      FirebaseJson gasJson;
      int sensorValue = analogRead(GAS_SENSOR_PIN);
      gasJson.add("name", "Gas sensor");
      gasJson.add("state", sensorValue);

      Serial.printf("Set dhtJson... %s\n\n", Firebase.setJSON(fbdo, "/test1/e3ba4698-d64b-447e-81f5-0bf0e09700eb/floors/e72cf36f-f9c5-4dee-b11a-951c0e3dc638/rooms/4d71d15e-9dc8-4e39-b506-facef152130a/DHT/1e1397cf-de32-4c6b-bc51-52b5ec8277ba/", dhtJson) ? "ok" : fbdo.errorReason().c_str());
      Serial.printf("Set gasJson... %s\n\n", Firebase.setJSON(fbdo, "/test1/e3ba4698-d64b-447e-81f5-0bf0e09700eb/floors/e72cf36f-f9c5-4dee-b11a-951c0e3dc638/rooms/4d71d15e-9dc8-4e39-b506-facef152130a/otherSensors/a5a13596-553e-4eed-8cf4-f676036f820d/", gasJson) ? "ok" : fbdo.errorReason().c_str());
    }
  //}


  if (dataChanged) {
    dataChanged = false;
    // When stream data is available, do anything here...
  }

  // After calling stream.keepAlive, now we can track the server connecting status
  if (!stream.httpConnected()) {
    // Server was disconnected!
  }
}