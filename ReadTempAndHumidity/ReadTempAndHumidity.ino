/**
 * DHT11 Sensor Reader
 * This sketch reads temperature and humidity data from the DHT11 sensor and prints the values to the serial port.
 * It also handles potential error states that might occur during reading.
 */
 
#include <DHT11.h>
#include <TimeLib.h>

DHT11 dht11(2);

char serialSend[100];

void setup() {
    // Initialize serial communication to allow debugging and data readout.
    // Using a baud rate of 9600 bps.
    Serial.begin(9600);
    dht11.setDelay(30000); // Set this to the desired delay. Default is 500ms.
}

void loop() {
    int temp = 0;
    int hum = 0;

    // Attempt to read the temperature and humidity values from the DHT11 sensor.
    int result = dht11.readTemperatureHumidity(temp, hum);

    // Check the results of the readings.
    // If the reading is successful, print the temperature and humidity values.
    // If there are errors, print the appropriate error messages.
    if (result == 0) {
        sprintf(serialSend, "temp=%d;hum=%d", temp, hum);
        Serial.println(serialSend);
    } else {
        // Print error message based on the error code.
        Serial.println(DHT11::getErrorString(result));
    }
}
