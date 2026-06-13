#include <Wire.h>
#include <MPU9250_WE.h>

#define MPU_ADDR 0x68

MPU9250_WE imu = MPU9250_WE(MPU_ADDR);

void setup() {
  Serial.begin(115200);
  delay(2000);   // IMPORTANT for ESP32

  Serial.println("ESP32 started");

  Wire.begin(21, 22);
  Wire.setClock(100000);

  Serial.println("Initializing MPU9250...");

  if (!imu.init()) {
    Serial.println("MPU9250 NOT FOUND");
    while (1);
  }

  Serial.println("MPU9250 FOUND");
}

void loop() {
  xyzFloat acc = imu.getAccRawValues();

  Serial.print("ACC: ");
  Serial.print(acc.x);
  Serial.print(", ");
  Serial.print(acc.y);
  Serial.print(", ");
  Serial.println(acc.z);

  delay(500);
}


