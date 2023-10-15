#include <Arduino.h>
#include "DHT.h"
#include "setup_project.h"
#include "write_data.h"
#include "listen_data_change.h"

#define RAIN_SENSOR_PIN A0
#define RAIN_SENSOR_THRESHOLD 500

#define DHTPIN 2  // D4
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

unsigned long sendDataPrevMillis = 0;

int count = 0;


int pinNumber[] = { 16, 5, 4 };

// void streamCallback(StreamData data) {
//   Serial.printf("sream path, %s\nevent path, %s\ndata type, %s\nevent type, %s\n\n",
//                 data.streamPath(),
//                 data.dataPath(),
//                 data.dataType(),
//                 data.eventType());

//   if (data.dataTypeEnum() == fb_esp_rtdb_data_type_array) {
//     // get array data from FirebaseData using FirebaseJsonArray object
//     FirebaseJsonArray *arr = data.to<FirebaseJsonArray *>();
//     for (size_t i = 0; i < arr->size(); i++) {
//       FirebaseJsonData result;
//       arr->get(result, i);
//       bool ledStatus = result.to<bool>();
//       if (ledStatus) {
//         digitalWrite(pinNumber[i], HIGH);
//       } else {
//         digitalWrite(pinNumber[i], LOW);
//       }
//       Serial.println(result.to<bool>());
//     }
//     arr->clear();
//   } else {
//     bool ledStatus = data.to<bool>();
//     int positionLed = data.dataPath().substring(1).toInt();
//     Serial.printf("/%d changed", positionLed);
//     if (ledStatus) {
//       digitalWrite(pinNumber[positionLed], HIGH);
//     } else {
//       digitalWrite(pinNumber[positionLed], LOW);
//     }
//   }

//   Serial.println();


//   Serial.printf("Received stream payload size: %d (Max. %d)\n\n", data.payloadLength(), data.maxPayloadLength());
//   dataChanged = true;
// }

// void streamTimeoutCallback(bool timeout) {
//   if (timeout)
//     Serial.println("stream timed out, resuming...\n");

//   if (!stream.httpConnected())
//     Serial.printf("error code: %d, reason: %s\n\n", stream.httpCode(), stream.errorReason().c_str());
// }

void setup() {
  Serial.begin(115200);

  setupWifi();

  setupFirebase();
  addHouse("Nhà của Hiền", "tranminhhien@gmail.com", "0922942242", "hien@242");

  listenChange();

  // if (!Firebase.beginStream(stream, "/devices/led"))
  //   Serial.printf("sream begin error, %s\n\n", stream.errorReason().c_str());

  // Firebase.setStreamCallback(stream, streamCallback, streamTimeoutCallback);

  // int lengthLed = sizeof(pinNumber) / sizeof(pinNumber[0]);
  // for (int i = 0; i < lengthLed; i++) {
  //   pinMode(pinNumber[i], OUTPUT);
  // }

  dht.begin();
}

void loop() {
  // if (Firebase.ready() && (millis() - sendDataPrevMillis > 2000 || sendDataPrevMillis == 0)) {
  //   float h = dht.readHumidity();
  //   float t = dht.readTemperature();
  //   sendDataPrevMillis = millis();
  //   count++;
  //   FirebaseJson dhtJson;
  //   dhtJson.add("temperature", t);
  //   dhtJson.add("humidity", h);
  //   if (t > 30) {
  //     dhtJson.add("message", "Cháy nhà!!!");
  //   } else {
  //     dhtJson.add("message", "");
  //   }

  //   FirebaseJson weatherJson;
  //   int sensorValue = analogRead(RAIN_SENSOR_PIN);
  //   if (sensorValue < RAIN_SENSOR_THRESHOLD) {
  //     weatherJson.add("status", "rain");
  //   } else {
  //     weatherJson.add("status", "sun");
  //   }

  //   Serial.printf("Set dhtJson... %s\n\n", Firebase.setJSON(fbdo, "/devices/DHT/", dhtJson) ? "ok" : fbdo.errorReason().c_str());
  //   Serial.printf("Set weatherJson... %s\n\n", Firebase.setJSON(fbdo, "/devices/weather/", weatherJson) ? "ok" : fbdo.errorReason().c_str());
  // }

  // if (dataChanged) {
  //   dataChanged = false;
  //   // When stream data is available, do anything here...
  // }

  // // After calling stream.keepAlive, now we can track the server connecting status
  // if (!stream.httpConnected()) {
  //   // Server was disconnected!
  // }
}
