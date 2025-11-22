#include "DHT.h"
#define DHT22_PIN 10

DHT dht22(DHT22_PIN, DHT22);

struct DataFormat{
  float humi;
  float temp;
};

struct DataFormat dht22_sensor_data = {0};

void read_temprature(){
  // read humidity
  dht22_sensor_data.humi  = dht22.readHumidity();
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
  }
}

void setup() {
  Serial.begin(9600);
  dht22.begin(); // initialize the DHT22 sensor
}

void loop() {
  read_temprature();
  delay(2000);
}
