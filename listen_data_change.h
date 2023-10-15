#include <FirebaseESP8266.h>
#include <addons/RTDBHelper.h>

String parentPath = "/test/e3ba4698-d64b-447e-81f5-0bf0e09700eb/floors/e72cf36f-f9c5-4dee-b11a-951c0e3dc638/rooms";
String childPath[] = {
  "/4d71d15e-9dc8-4e39-b506-facef152130a/devices/6209e828-a8f8-4e02-bbeb-06f5c9ac614f/state",
  "/4d71d15e-9dc8-4e39-b506-facef152130a/devices/72e2d23f-2602-4071-947f-38ee4fe5ef7e/state",
  "/4d71d15e-9dc8-4e39-b506-facef152130a/devices/c28adf60-4dfa-446c-8781-d3afb4377c05/state"
};
int pins[] = { 16, 5, 4 };

volatile bool dataChanged = false;

void streamCallback(MultiPathStreamData stream) {
  size_t numChild = sizeof(childPath) / sizeof(childPath[0]);
  for (size_t i = 0; i < numChild; i++) {
    if (stream.get(childPath[i])) {
      pinMode(pins[i], OUTPUT);
      digitalWrite(pins[i], stream.value == "0" ? LOW : HIGH);
    }
  }
  Serial.println();
  Serial.printf("Received stream payload size: %d (Max. %d)\n\n", stream.payloadLength(), stream.maxPayloadLength());
  dataChanged = true;
}

void streamTimeoutCallback(bool timeout) {
  if (timeout)
    Serial.println("stream timed out, resuming...\n");

  if (!stream.httpConnected())
    Serial.printf("error code: %d, reason: %s\n\n", stream.httpCode(), stream.errorReason().c_str());
}

void listenChange() {
  if (!Firebase.beginMultiPathStream(stream, parentPath))
    Serial.printf("sream begin error, %s\n\n", stream.errorReason().c_str());

  Firebase.setMultiPathStreamCallback(stream, streamCallback, streamTimeoutCallback);
}
