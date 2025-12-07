# Intro
This project involves the development of a microcontroller prototype designed for the real-time monitoring of temperature and humidity. The system is built to provide data feedback in a versatile and immediate manner. Information is presented locally in two formats: text on an LCD display and graphics on a TFT screen. Additionally, the system is equipped with wireless communication (Bluetooth), which allows for the remote consultation of monitoring data via an external device, facilitating portability and continuous analysis.

# Hardware Requirements

Microcontroller: Arduino Uno (or compatible)

Sensors: DHT22 (Temperature & Humidity)

Displays:

    LCD Display (16x2)

    TFT Display (1.8" ST7735 driver)

Communication: Bluetooth Module (HC-05 or HC-06)

Shutterstock

# Pin Configuration

The code is configured with the following pin assignments:
Component	Pin Function	Arduino Pin
    DHT22	Data	A2
    LCD	RS	7
    Enable (E)	6
    D4	5
    D5	4
    D6	3
    D7	2
    TFT (ST7735)	CS	10
    RST	9
    DC	8
    Bluetooth	RX (SoftwareSerial)	A0
    TX (SoftwareSerial)	A1
# Dependencies
To compile this project, you must install the following libraries via the Arduino Library Manager or by downloading the source files:

    DHT Sensor Library by Adafruit (requires Adafruit Unified Sensor as a dependency):

        https://github.com/adafruit/DHT-sensor-library

    Adafruit GFX Library (Core graphics library for displays):

        https://github.com/adafruit/Adafruit-GFX-Library

    Adafruit ST7735 and ST7789 Library (Hardware specific library for the TFT):

        https://github.com/adafruit/Adafruit-ST7735-Library

    Virtuino (For Bluetooth communication with the Virtuino App):

        https://github.com/iliaslamprou/Virtuino (Note: Ensure you have the version matching VirtuinoBluetooth.h)

    Standard Arduino Libraries (Included with IDE):

        LiquidCrystal.h

        SoftwareSerial.h

        SPI.h

# Virtuino App Configuration

This project interacts with the Virtuino mobile app. The following Virtual Pins are mapped in the code:

    V0 (Write): Temperature data sent to App.

    V1 (Write): Humidity data sent to App.

    V2 (Read): Delay interval (in ms) controlled by the App sliders/input.

# References

    DHT22 Implementation: Basic logic derived from ArduinoGetStarted - DHT22 Tutorial.

    Virtuino Platform: Documentation and app details at Virtuino.com.

    Adafruit Graphics: Graphics primitives documentation at Adafruit GFX.

# License

This project is released under the terms of the GNU General Public License, Version 3 (GPLv3). This means the source code is free software: you can redistribute it and/or modify it under the terms of the GPLv3. You should have received a copy of the GNU General Public License along with this program. If not, see https://www.gnu.org/licenses/gpl-3.0.html.