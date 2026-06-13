//RECEIVER

HardwareSerial LoRaSerial(2);

void setup() {
  Serial.begin(115200);
  LoRaSerial.begin(9600, SERIAL_8N1, 16, 17);
  Serial.println("LoRa Receiver Ready");
}

void loop() {
  if (LoRaSerial.available()) {
    String msg = LoRaSerial.readStringUntil('\n');
    Serial.print("Received: ");
    Serial.println(msg);
  }
}
