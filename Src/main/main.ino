#include "DHT.h"
#define DHT22_PIN 10
//////////////////////////////////////////////
//        RemoteXY include library          //
//////////////////////////////////////////////

// you can enable debug logging to Serial at 115200
//#define REMOTEXY__DEBUGLOG

// RemoteXY select connection mode and include library
#define REMOTEXY_MODE__SOFTSERIAL

#include <SoftwareSerial.h>

// RemoteXY connection settings
#define REMOTEXY_SERIAL_RX 2
#define REMOTEXY_SERIAL_TX 3
#define REMOTEXY_SERIAL_SPEED 9600

#include <RemoteXY.h>

// RemoteXY GUI configuration
#pragma pack(push, 1)

uint8_t RemoteXY_CONF[] =   // 91 bytes
  { 255,0,0,16,0,84,0,19,0,0,0,83,77,65,73,0,24,1,106,200,
  1,1,6,0,68,5,21,98,62,1,8,36,67,13,170,26,19,78,2,26,
  2,67,59,170,26,19,78,2,26,2,129,10,157,34,9,64,17,72,117,109,
  105,100,121,0,129,49,157,56,9,64,17,84,101,109,112,101,114,97,116,117,
  114,101,0,68,5,83,98,64,1,8,36 };
  
// this structure defines all the variables and events of your control interface 
struct {
    // output variables
  float graph_temp;
  float humd;
  float temperature;
  float graph_hum;
  uint8_t connect_flag;  // =1 if wire connected, else =0

} RemoteXY;   
#pragma pack(pop)

DHT dht22(DHT22_PIN, DHT22);

struct DataFormat {
  float humi;
  float temp;
};

struct DataFormat dht22_sensor_data = { 0 };

void read_ambient_sensor() {
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
  }
}

void setup() {
  Serial.begin(9600);
  RemoteXY_Init();
  dht22.begin();  // initialize the DHT22 sensor
}

void loop() {
  RemoteXY_Handler();
  read_ambient_sensor();
  RemoteXY.temperature = dht22_sensor_data.temp;
  RemoteXY.humd = dht22_sensor_data.humi;
  RemoteXY.graph_temp = dht22_sensor_data.temp;
  RemoteXY.graph_hum = dht22_sensor_data.humi;
  RemoteXY_delay(2000);
}
