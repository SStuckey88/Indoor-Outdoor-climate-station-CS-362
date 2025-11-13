/**
 * DHT11 Sensor Reader
 * This sketch reads temperature and humidity data from the DHT11 sensor and prints the values to the serial port.
 * It also handles potential error states that might occur during reading.
 */

 //Sending Serial data to the Pi is done by using the first byte/char as the message TYPE
 //Possible data types (anything without one of these will be ignored):
 //D -> DATA, followed by the weather data
 //E -> ERROR, followed by the error message
 //J -> JOYSTICK, followed by the joystick values

#include <Wire.h>
#include "SparkFunBME280.h"
#include <DHT11.h>
#include <TimeLib.h>

BME280 pressureSensor;
DHT11 dht11(2);

int JoyXPin = A0;
int JoyYPin = A1;

char serialSend[100];

int temp = 0;
int hum = 0;
float barPres = 0.0;

unsigned long previousMillis = 0;
unsigned long previousJoystickMillis = 0;
long interval = 100;

int xRead = 0;
int yRead = 0;
int xVal = 0;
int yVal = 0;
int xPrev = 0;
int yPrev = 0;

void setup() {
    Serial.begin(9600);

    Wire.begin();

    dht11.setDelay(0);
    
    pressureSensor.setI2CAddress(0x76);
    if(pressureSensor.beginI2C() == false) Serial.println("EBarometric Pressure sensor connection failed");
}

void loop() {

    unsigned long currentMillis = millis();

    //Read Joystick data every 100ms - send to Pi if changed since last read
    if (currentMillis - previousJoystickMillis >= interval) {
        xRead = analogRead(JoyXPin);
        yRead = analogRead(JoyYPin);
        xVal = map(xRead, 0, 1023, -1, 1);
        yVal = map(yRead, 0, 1023, 1, -1);
        if(xVal != xPrev || yVal != yPrev) {
            sprintf(serialSend, "Jx=%d;y=%d", xVal, yVal);
            Serial.println(serialSend);
            xPrev = xVal;
            yPrev = yVal;
        }
        previousJoystickMillis = currentMillis;
    }

    //Timer for taking measurements
    if (currentMillis - previousMillis >= (interval*50)) {

        int result = dht11.readTemperatureHumidity(temp, hum);
        barPres = pressureSensor.readFloatPressure();

        if (result == 0) {
            sprintf(serialSend, "Dtemp=%d;hum=%d;pres=%.0f", temp, hum, barPres);
            Serial.println(serialSend);
        } else {
            // Print error message based on the error code.
            Serial.println("ETemp/Humidity sensor connection failed");
        }
        previousMillis = currentMillis;
    }
}
    
