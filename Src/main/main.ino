#include "DHT.h"
#include "VirtuinoBluetooth.h"
#include <SoftwareSerial.h>
#include <Adafruit_GFX.h>    // Biblioteca Gráfica
#include <Adafruit_ST7735.h> // Biblioteca do Ecrã TFT
#include <LiquidCrystal.h>   // Biblioteca do Ecrã LCD
#include <SPI.h>
// --- CONFIGURAÇÃO DO DHT22 ---
// PIN 12 was in confligct with the TFT setup
#define DHT22_PIN A2

// --- CONFIGURAÇÃO DO LCD (VERDE) ---
// Pinos: RS=7, E=6, D4=5, D5=4, D6=3, D7=2
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

// --- CONFIGURAÇÃO DO TFT (COLORIDO) ---
#define TFT_CS     10
#define TFT_RST    9  
#define TFT_DC     8

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
SoftwareSerial bluetoothSerial = SoftwareSerial(A0, A1); // arduino RX pin=2 arduino TX pin=3 connect the arduino RX pin to bluetooth module TX pin - connect the arduino TX pin to bluetooth module RX pin. Disable this line if you want to use hardware serialVirtuinoBluetooth virtuino(bluetoothSerial); // Set SoftwareSerial baud rate. - Disable this line if you want to use hardware serial
VirtuinoBluetooth virtuino(bluetoothSerial); // Set SoftwareSerial baud rate. - Disable this line if you want to use hardware serial
DHT dht22(DHT22_PIN, DHT22);

struct DataFormat {
  float humi;
  float temp;
};

int delay_variable = 2000;
long last_millis = 0;

struct DataFormat dht22_sensor_data = { 0 };

void display_data(){
  // --- ATUALIZAR LCD ---
  lcd.setCursor(10, 1);
  lcd.print(dht22_sensor_data.humi);
  lcd.print("%");
  lcd.setCursor(6, 0);
  lcd.print(dht22_sensor_data.temp);
  lcd.print("C");
  // --- ATUALIZAR TFT ---
  tft.fillScreen(ST7735_BLACK);
  tft.setCursor(0, 70);
  tft.setTextColor(ST7735_WHITE);
  tft.setTextSize(1);
  tft.print("Humididty:");
  tft.println(dht22_sensor_data.humi);
  tft.println("");
  tft.print("Temp:");
  tft.print(dht22_sensor_data.temp);
}

void setup_display(){
  // --- INICIAR LCD ---
  lcd.begin(16, 2);
  lcd.print("Temp: ");
  lcd.setCursor(0, 1);
  lcd.print("Humidity: ");
  // --- INICIAR TFT ---
  tft.initR(INITR_BLACKTAB); 
  tft.setRotation(2); 
  // Limpar ecrã
  tft.fillScreen(ST7735_BLACK);
}

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
  last_millis = millis();
  setup_display();
  bluetoothSerial.begin(9600);  // Enable this line if you want to use software serial (UNO, Nano etc.)
  //Serial1.begin(9600); // Enable this line if you want to use hardware serial (Mega, DUE etc.)
  virtuino.DEBUG = false;  // set this value TRUE to enable the serial monitor status
  dht22.begin();          // initialize the DHT22 sensor
}

void loop() {
  virtuino.run();
  delay_variable = virtuino.vMemoryRead(2);
  if(millis() - last_millis >= delay_variable){
    read_temprature();
    display_data();
    last_millis = millis();
    Serial.print("Interval: ");
    Serial.print(delay_variable);
    Serial.print(" ms\n");                 
  }
}
