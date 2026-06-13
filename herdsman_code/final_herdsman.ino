#include "painlessMesh.h"
#include <TinyGPSPlus.h>

// ---------------- MESH ----------------
#define MESH_PREFIX "FarmMesh"
#define MESH_PASSWORD "12345678"
#define MESH_PORT 5555

Scheduler userScheduler;
painlessMesh mesh;

// ---------------- GPS ----------------
TinyGPSPlus gps;

#define GPS_SERIAL Serial1

// ---------------- LORA ----------------
#define LORA_SERIAL Serial2

String latestGPS = "GPS_NOT_LOCKED";

// ---------------- RECEIVE CALLBACK ----------------
void receivedCallback(uint32_t from, String &msg) {

  Serial.println("=================================");
  Serial.print("Received from Node: ");
  Serial.println(from);

  Serial.println(msg);

  // Add GPS to outgoing LoRa packet
  String finalPacket = msg + "," + latestGPS;

  // Send via LoRa
  LORA_SERIAL.println(finalPacket);

  Serial.println("LoRa Sent:");
  Serial.println(finalPacket);
}

// ---------------- SETUP ----------------
void setup() {

  Serial.begin(115200);

  // -------- GPS --------
  GPS_SERIAL.begin(9600, SERIAL_8N1, 32, -1);

  // -------- LoRa --------
  LORA_SERIAL.begin(9600, SERIAL_8N1, 16, 17);

  // -------- MESH --------
  mesh.init(MESH_PREFIX,
            MESH_PASSWORD,
            &userScheduler,
            MESH_PORT);

  mesh.onReceive(&receivedCallback);

  Serial.println("=================================");
  Serial.println("HERDSMAN GATEWAY READY");
  Serial.println("Waiting for GPS...");
  Serial.println("=================================");
}

// ---------------- LOOP ----------------
void loop() {

  mesh.update();

  // -------- GPS UPDATE --------
  while (GPS_SERIAL.available()) {

    char c = GPS_SERIAL.read();

    gps.encode(c);

    // Print raw GPS characters
    Serial.write(c);
  }

  // -------- GPS STATUS --------
  if (gps.location.isValid()) {

    latestGPS =
      "LAT=" + String(gps.location.lat(), 6) +
      ",LON=" + String(gps.location.lng(), 6);

  } else {

    latestGPS = "GPS_WAITING_FOR_SIGNAL";
  }

  // -------- OPTIONAL DEBUG --------
  static unsigned long lastDebug = 0;

  if (millis() - lastDebug > 5000) {

    lastDebug = millis();

    Serial.println();
    Serial.println("========== GPS STATUS ==========");

    if (gps.location.isValid()) {

      Serial.println("GPS LOCKED");

      Serial.print("Latitude: ");
      Serial.println(gps.location.lat(), 6);

      Serial.print("Longitude: ");
      Serial.println(gps.location.lng(), 6);

      Serial.print("Satellites: ");
      Serial.println(gps.satellites.value());

    } else {

      Serial.println("GPS connected but waiting for satellites...");
      Serial.println("This is NORMAL indoors.");

      Serial.print("Satellites Found: ");
      Serial.println(gps.satellites.value());
    }

    Serial.println("================================");
  }
}