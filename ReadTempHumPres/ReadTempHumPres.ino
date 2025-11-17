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
#include "AGS02MA.h"
#include <TimeLib.h>

BME280 pressureSensor;
DHT11 dht11(2);
AGS02MA AGS(26);

int JoyXPin = A0;
int JoyYPin = A1;

char serialSend[100];

int temp = 0;
int hum = 0;
int tvoc = 0;
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

    dht11.setDelay(10);
    
    pressureSensor.setI2CAddress(0x76);
    if(pressureSensor.beginI2C() == false) Serial.println("EBarometric Pressure sensor connection failed");

    setSyncProvider(requestTimeSync); 
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

    if (currentMillis - previousMillis >= (interval*50)) {
        if(timeStatus() == timeSet) {
            time_t time = now();
            unsigned long seconds = (unsigned long) time;
            int result = dht11.readTemperatureHumidity(temp, hum);
            barPres = pressureSensor.readFloatPressure();
            tvoc = AGS.readPPB();

            if (result == 0) {
                sprintf(serialSend, "Dtime=%lu;temp=%d;hum=%d;pres=%.0f;tvoc=%d", seconds, temp, hum, barPres, tvoc);
                Serial.println(serialSend);
            } else {
                // Print error message based on the error code.
                Serial.println("ETemp/Humidity sensor connection failed");
            }
            previousMillis = currentMillis;
        }
    }
    if (Serial.available()) {
        processSyncMessage();
    } 
}

time_t requestTimeSync()
{
  Serial.println("T");  
  return 0; // the time will be sent later in response to serial mesg
}

void processSyncMessage() {
  unsigned long pctime;

  if(Serial.find('T')) {
    //setTime(int hr,int min,int sec,int day, int month, int yr);
     pctime = Serial.parseInt();
    setTime(pctime); // Sync Arduino clock to the time received on the serial port
  }
}
    
