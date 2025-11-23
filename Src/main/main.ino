#include "DHT.h"
#define DHT22_PIN 10
#include "VirtuinoBluetooth.h"
#include <SoftwareSerial.h>

SoftwareSerial bluetoothSerial = SoftwareSerial(2, 3); // arduino RX pin=2 arduino TX pin=3 connect the arduino RX pin to bluetooth module TX pin - connect the arduino TX pin to bluetooth module RX pin. Disable this line if you want to use hardware serialVirtuinoBluetooth virtuino(bluetoothSerial); // Set SoftwareSerial baud rate. - Disable this line if you want to use hardware serial
VirtuinoBluetooth virtuino(bluetoothSerial); // Set SoftwareSerial baud rate. - Disable this line if you want to use hardware serial
DHT dht22(DHT22_PIN, DHT22);

struct DataFormat {
  float humi;
  float temp;
};

struct DataFormat dht22_sensor_data = { 0 };

void read_temprature() {
  // read humidity
  dht22_sensor_data.humi = dht22.readHumidity();
  // read temperature
  dht22_sensor_data.temp = dht22.readTemperature();
  // check if any reads failed
  if (isnan(dht22_sensor_data.humi) || isnan(dht22_sensor_data.temp)) {
    Serial.println("Failed to read from DHT22 sensor!");
  } else {
    Serial.print("[DHT22] Humidity: ");
    Serial.print(dht22_sensor_data.humi);
    Serial.print("%");
    Serial.print("  |  ");
    Serial.print("Temperature: ");
    Serial.print(dht22_sensor_data.temp);
    Serial.print("°C\n");
    virtuino.vMemoryWrite(0, dht22_sensor_data.temp);  // write temperature 1 to virtual pin V0. On Virtuino panel add a value display or an analog instrument to pin V0
    virtuino.vMemoryWrite(1, dht22_sensor_data.humi);  // write temperature 1 to virtual pin V1. On Virtuino panel add a value display or an
  }
}

void setup() {
  Serial.begin(9600);
  bluetoothSerial.begin(9600);  // Enable this line if you want to use software serial (UNO, Nano etc.)
  //Serial1.begin(9600); // Enable this line if you want to use hardware serial (Mega, DUE etc.)
  virtuino.DEBUG = true;  // set this value TRUE to enable the serial monitor status
  dht22.begin();          // initialize the DHT22 sensor
}

void loop() {
  virtuino.run();               
  read_temprature();
}
