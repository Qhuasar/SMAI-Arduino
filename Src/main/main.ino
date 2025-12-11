#include "DHT.h"
#include "VirtuinoBluetooth.h"
#include <SoftwareSerial.h>
#include <TFT.h>             // Biblioteca do Ecrã TFT
#include <LiquidCrystal.h>   // Biblioteca do Ecrã LCD
#include <SPI.h>

#define DELAY_MAX_TIME 15000
#define DELAY_MIN_TIME 2000  // Corrigido "DEYLAY" para "DELAY"

// --- CONFIGURAÇÃO DO DHT22 ---
#define DHT22_PIN A2

// --- CONFIGURAÇÃO DO LCD (VERDE) ---
// Pinos: RS=7, E=6, D4=5, D5=4, D6=3, D7=2
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

// --- CONFIGURAÇÃO DO TFT (COLORIDO) ---
#define TFT_CS     10
#define TFT_RST    9  
#define TFT_DC     8

// TFT Variáveis de Desenho
int xPos = 0;
int lastY = 0;
bool firstPoint = true;

// Inicialização do TFT
TFT TFTscreen = TFT(TFT_CS, TFT_DC, TFT_RST);

// --- CONFIGURAÇÃO BLUETOOTH & VIRTUINO ---
// Definir portas virtuais RX=A0, TX=A1
SoftwareSerial bluetoothSerial(A0, A1); 
// Inicializar a biblioteca Virtuino com a porta série virtual
VirtuinoBluetooth virtuino(bluetoothSerial); 

// Inicialização do DHT
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
  // Mapear temperatura (0 a 40ºC) para a altura do ecrã
  int yPos = map((int)dht22_sensor_data.temp, 0, 40, TFTscreen.height(), 10);
  
  TFTscreen.stroke(255, 255, 0); // Linha amarela
  if (firstPoint) {
    TFTscreen.point(xPos, yPos);
    firstPoint = false;
  } else {
    TFTscreen.line(xPos - 1, lastY, xPos, yPos);
  }

  lastY = yPos;
  xPos++;

  // Reset do Ecrã quando chega ao fim
  if (xPos >= TFTscreen.width()) {
    xPos = 0;
    firstPoint = true;
    TFTscreen.background(0, 0, 0);
    TFTscreen.stroke(255, 255, 255);
    TFTscreen.text("Temp Graph (0-40C)", 0, 0);
  }
}

void setup_display(){
  // --- INICIAR LCD ---
  lcd.begin(16, 2);
  lcd.print("Temp: ");
  lcd.setCursor(0, 1);
  lcd.print("Humidity: ");
  
  // --- INICIAR TFT ---
  TFTscreen.begin();
  TFTscreen.background(0, 0, 0); // Fundo Preto
  TFTscreen.stroke(255, 255, 255); // Texto Branco
  TFTscreen.setTextSize(1);
  TFTscreen.text("Temp Graph (0-40C)", 0, 0);
}

void read_temprature() {
  // Ler humidade
  dht22_sensor_data.humi = dht22.readHumidity();
  // Ler temperatura
  dht22_sensor_data.temp = dht22.readTemperature();

  // Verificar falhas de leitura
  if (isnan(dht22_sensor_data.humi) || isnan(dht22_sensor_data.temp)) {
    Serial.println("Failed to read from DHT22 sensor!");
  } else {
    // Imprimir na porta Serie (USB)
    Serial.print("[DHT22] Humidity: ");
    Serial.print(dht22_sensor_data.humi);
    Serial.print("%");
    Serial.print("  |  ");
    Serial.print("Temperature: ");
    Serial.print(dht22_sensor_data.temp);
    Serial.print("°C\n");
    
    // Enviar para a APP Virtuino
    virtuino.vMemoryWrite(0, dht22_sensor_data.temp);  // Pino Virtual V0 = Temperatura
    virtuino.vMemoryWrite(1, dht22_sensor_data.humi);  // Pino Virtual V1 = Humidade
  }
}

void setup() {
  Serial.begin(9600);
  last_millis = millis();
  
  setup_display();
  
  bluetoothSerial.begin(9600);  // Velocidade do módulo HC-05 (normalmente 9600)
  virtuino.DEBUG = false;       // Colocar a true se quiser ver debug da Virtuino no Serial Monitor
  dht22.begin();                // Inicializar sensor
}

void loop() {
  virtuino.run(); // Processar comandos Bluetooth
  
  // Ler o atraso definido na App (Pino Virtual V2)
  delay_variable = virtuino.vMemoryRead(2);
  
  // Garantir que o atraso está dentro dos limites seguros
  delay_variable = constrain(delay_variable, DELAY_MIN_TIME, DELAY_MAX_TIME); 

  // Temporizador não bloqueante (millis)
  if(millis() - last_millis >= delay_variable){
    read_temprature();
    display_data();
    last_millis = millis();
    Serial.print("Interval: ");
    Serial.print(delay_variable);
    Serial.print(" ms\n");                 
  }
}