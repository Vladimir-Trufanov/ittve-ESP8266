#include <ESP8266WiFi.h>

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    // Wait for serial port to connect. Needed for native USB port only
    delay(100);
  }

  Serial.println("ESP8266 Parameters:");
  Serial.println("-------------------");

  // Информация о чипе
  Serial.print("Chip ID: ");
  Serial.println(ESP.chipId());

  // Информация о CPU
  Serial.print("CPU Frequency: ");
  Serial.print(System.cpuFreq() / 1000000); // Convert to MHz
  Serial.println(" MHz");

  // Информация о Flash-памяти
  Serial.print("Flash Size: ");
  Serial.println(ESP.flashSize() / (1024 * 1024)); // Convert to MB

  // MAC-адрес
  Serial.print("MAC Address: ");
  Serial.println(WiFi.macAddress());

  Serial.println("-------------------");
}

void loop() {
  // Основной код программы
}
