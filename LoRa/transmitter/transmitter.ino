//TRANSMITTER

HardwareSerial LoRaSerial(2);

unsigned long int message_counter=0;

void setup() {
  Serial.begin(115200);
  LoRaSerial.begin(9600, SERIAL_8N1, 16, 17);
  Serial.println("LoRa Sender Ready");
}

void loop() {
  LoRaSerial.print("HELLO FROM ESP32");
  LoRaSerial.print(" ");
  LoRaSerial.println(message_counter++);
  
  Serial.println("Packet Sent");
  delay(200);
}
