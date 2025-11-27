#include "DHT.h"
#define DHT22_PIN 10
#include "VirtuinoBluetooth.h"
#include <SoftwareSerial.h>
#include <Adafruit_GFX.h>    // Biblioteca Gráfica
#include <Adafruit_ST7735.h> // Biblioteca do Ecrã TFT
#include <LiquidCrystal.h>   // Biblioteca do Ecrã LCD
#include <SPI.h>

// --- CONFIGURAÇÃO DO LCD (VERDE) ---
// Pinos: RS=7, E=6, D4=5, D5=4, D6=3, D7=2
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

// --- CONFIGURAÇÃO DO TFT (COLORIDO) ---
#define TFT_CS     10
#define TFT_RST    9  
#define TFT_DC     8

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
SoftwareSerial bluetoothSerial = SoftwareSerial(2, 3); // arduino RX pin=2 arduino TX pin=3 connect the arduino RX pin to bluetooth module TX pin - connect the arduino TX pin to bluetooth module RX pin. Disable this line if you want to use hardware serialVirtuinoBluetooth virtuino(bluetoothSerial); // Set SoftwareSerial baud rate. - Disable this line if you want to use hardware serial
VirtuinoBluetooth virtuino(bluetoothSerial); // Set SoftwareSerial baud rate. - Disable this line if you want to use hardware serial
DHT dht22(DHT22_PIN, DHT22);

struct DataFormat {
  float humi;
  float temp;
};

struct DataFormat dht22_sensor_data = { 0 };

void display_data(){
  // --- ATUALIZAR LCD ---
  lcd.setCursor(0, 1);
  lcd.print("Tempo: ");
  lcd.print(millis() / 1000);

  // --- ATUALIZAR TFT ---
  // Quadrado a piscar no centro
  int cor = random(0xFFFF); 
  tft.fillRect(40, 80, 50, 40, cor);
  
  tft.setCursor(45, 95);
  tft.setTextColor(ST7735_BLACK);
  tft.setTextSize(1);
  tft.print(millis() / 1000);
}

void setup_display(){
  // --- INICIAR LCD ---
  lcd.begin(16, 2);
  lcd.print("Teste SMAI");

  // --- INICIAR TFT ---
  tft.initR(INITR_BLACKTAB); 
  
  // *** AQUI ESTÁ A CORREÇÃO DA ROTAÇÃO ***
  // 0 = Normal
  // 1 = Deitado (90 graus)
  // 2 = Invertido (180 graus) -> Tenta este!
  // 3 = Deitado Invertido (270 graus)
  tft.setRotation(2); 

  // Limpar ecrã
  tft.fillScreen(ST7735_BLACK);
  
  // Desenhar moldura Branca
  tft.drawRect(0, 0, 128, 160, ST7735_WHITE);
  
  // Escrever Título
  tft.setCursor(15, 20);
  tft.setTextColor(ST7735_CYAN);
  tft.setTextSize(2); 
  tft.println("PROJETO");
  
  tft.setCursor(35, 45);
  tft.setTextColor(ST7735_YELLOW);
  tft.setTextSize(2);
  tft.println("SMAI");
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
  setup_display();
  bluetoothSerial.begin(9600);  // Enable this line if you want to use software serial (UNO, Nano etc.)
  //Serial1.begin(9600); // Enable this line if you want to use hardware serial (Mega, DUE etc.)
  virtuino.DEBUG = true;  // set this value TRUE to enable the serial monitor status
  dht22.begin();          // initialize the DHT22 sensor
}

void loop() {
  virtuino.run();               
  read_temprature();
  display_data();
}
