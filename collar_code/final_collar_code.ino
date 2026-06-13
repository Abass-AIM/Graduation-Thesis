#include "painlessMesh.h"
#include <Wire.h>
#include <SparkFunMPU9250-DMP.h>

#define COLLAR_ID "COLLAR_1"

// ---------------- MESH ----------------
#define MESH_PREFIX "FarmMesh"
#define MESH_PASSWORD "12345678"
#define MESH_PORT 5555

Scheduler userScheduler;
painlessMesh mesh;

// ---------------- MPU ----------------
MPU9250_DMP imu;
bool imuOK = false;

// ---------------- THERMISTOR ----------------
#define THERM_PIN 34

// ---------------- DATA ----------------
float ax = 0, ay = 0, az = 0;

// ---------------- TIMER ----------------
unsigned long lastSend = 0;
const unsigned long sendInterval = 2000;

// ---------------- IMU READ FUNCTION ----------------
void readIMU() {

  if (!imu.dataReady()) return;

  if (imu.update(UPDATE_ACCEL) == INV_SUCCESS) {

    // Convert raw accel to "g"
    ax = imu.ax / 16384.0;
    ay = imu.ay / 16384.0;
    az = imu.az / 16384.0;
  }
}

unsigned int dataCounter=0;

void setup() {

  Serial.begin(115200);
  delay(1000);

  // -------- I2C --------
  Wire.begin(21, 22);
  Wire.setClock(100000);
  delay(200);

  // -------- MESH --------
  mesh.init(MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT);
  Serial.println("Mesh Started");

  // -------- MPU --------
  Serial.println("Initializing MPU9250...");

  if (imu.begin() == INV_SUCCESS) {

    imu.setSensors(INV_XYZ_ACCEL);
    imu.setAccelFSR(2);

    imuOK = true;
    Serial.println("MPU9250 READY");
  } else {
    imuOK = false;
    Serial.println("MPU9250 FAILED");
  }
}

void loop() {

  mesh.update();

  // -------- IMU UPDATE --------
  if (imuOK) {
    readIMU();
  }

  // -------- SEND DATA --------
  if (millis() - lastSend >= sendInterval) {

    lastSend = millis();
    sendSensorData();
  }
}

void sendSensorData() {

  // -------- THERMISTOR --------
  int raw = analogRead(THERM_PIN);
  float voltage = raw * (3.3 / 4095.0);
  float temperature = voltage * 10.0;

  // -------- MESSAGE --------
  String msg =
    String(dataCounter++) + "," + String(COLLAR_ID) +
    ",TEMP=" + String(temperature, 2) +
    ",AX=" + String(ax, 3) +
    ",AY=" + String(ay, 3) +
    ",AZ=" + String(az, 3);

  mesh.sendBroadcast(msg);

  Serial.println("Sent:");
  Serial.println(msg);
}