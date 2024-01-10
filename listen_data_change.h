#include <FirebaseESP8266.h>
#include <addons/RTDBHelper.h>

String parentPath = "/test1/e3ba4698-d64b-447e-81f5-0bf0e09700eb";
String childPath[] = {
  "/floors"
  // "devices/72e2d23f-2602-4071-947f-38ee4fe5ef7e/state", //kitchen
  // "/floors/e72cf36f-f9c5-4dee-b11a-951c0e3dc638/rooms/4e9473a3-3cd9-45b9-b33b-bf264cf43e04/devices/a76be91b-8087-4987-a94e-0419756cd748/state", //living
  // "/floors/e72cf36f-f9c5-4dee-b11a-951c0e3dc638/rooms/ae42c2e9-eb8c-4777-b89b-cce821206550/devices/494c63c9-1d6e-4535-94d7-c1145757807d/state", // floor1 - room1
  // "/floors/40b38a85-36ea-4a72-921a-2fbb579a2daf/rooms/2de1b963-b2c3-48bc-be79-81976b2b6ab9/devices/896c0b60-590d-4634-b132-8f5eab39044c/state", // floor2 - room1
  // "/floors/40b38a85-36ea-4a72-921a-2fbb579a2daf/rooms/3fdfefc2-8041-4416-848b-3c300891f9b3/devices/b9a23198-179b-42a8-b05e-58bded59c2c9/state" // floor2 - room1
};


int pins[] = { 16, 5, 4, 0, 2 };  // D0, 1, 2, 3, 4

volatile bool dataChanged = false;

void streamCallback(MultiPathStreamData stream) {
  size_t numChild = sizeof(childPath) / sizeof(childPath[0]);
  for (size_t i = 0; i < numChild; i++) {
    if (stream.get(childPath[i])) {
      // if (i == 0) {
      //   pinMode(D6, OUTPUT);
      //   pinMode(D7, OUTPUT);
      //   pinMode(D8, OUTPUT);
      //   digitalWrite(D6, HIGH);
      //   digitalWrite(D7, LOW);
      //   if (stream.value == "0") analogWrite(pins[i], 0);
      //   else if (stream.value == "1") analogWrite(pins[i], 85);
      //   else if (stream.value == "2") analogWrite(pins[i], 170);
      //   else if (stream.value == "3") analogWrite(pins[i], 255);
      // } else {
        // pinMode(pins[i], OUTPUT);
        // digitalWrite(pins[i], stream.value == "0" ? LOW : HIGH);
      // }
    }
  }

  Serial.println();
  Serial.printf("Received stream payload size: %d (Max. %d)\n\n", stream.payloadLength(), stream.maxPayloadLength());
  dataChanged = true;
}

// void streamCallback(MultiPathStreamData stream)
// {
//   size_t numChild = sizeof(childPath) / sizeof(childPath[0]);

//   for (size_t i = 0; i < numChild; i++)
//   {
//     if (stream.get(childPath[i]))
//     {
//       Serial.printf("path: %s, event: %s, type: %s, value: %s%s", stream.dataPath.c_str(), stream.eventType.c_str(), stream.type.c_str(), stream.value.c_str(), i < numChild - 1 ? "\n" : "");
//     }
//   }

//   Serial.println();
//   Serial.printf("Received stream payload size: %d (Max. %d)\n\n", stream.payloadLength(), stream.maxPayloadLength());

//   dataChanged = true;
// }

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

// int pins[] = { 16, 5, 4, 0, 2 };
// String path = "/test1/e3ba4698-d64b-447e-81f5-0bf0e09700eb/floors";
// volatile bool dataChanged = false;


// void streamCallback(StreamData data) {
//   Serial.printf("sream path, %s\nevent path, %s\ndata type, %s\nevent type, %s\n\n",
//                 data.streamPath().c_str(),
//                 data.dataPath().c_str(),
//                 data.dataType().c_str(),
//                 data.eventType().c_str());
//   printResult(data);  // see addons/RTDBHelper.h

//   Serial.printf("Received stream payload size: %d (Max. %d)\n\n", data.payloadLength(), data.maxPayloadLength());

//   dataChanged = true;
// }

// void streamTimeoutCallback(bool timeout) {
//   if (timeout)
//     Serial.println("stream timed out, resuming...\n");

//   if (!stream.httpConnected())
//     Serial.printf("error code: %d, reason: %s\n\n", stream.httpCode(), stream.errorReason().c_str());
// }

// void listenChange() {
//   if (!Firebase.beginStream(stream, path))
//     Serial.printf("sream begin error, %s\n\n", stream.errorReason().c_str());

//   Firebase.setStreamCallback(stream, streamCallback, streamTimeoutCallback);
// }
