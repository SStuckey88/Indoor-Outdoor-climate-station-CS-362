/*
  TLS WiFi Web client

  Board CA Root certificate bundle is embedded inside WiFi firmware:
  https://github.com/arduino/uno-r4-wifi-usb-bridge/blob/main/certificates/cacrt_all.pem

  Find the full UNO R4 WiFi Network documentation here:
  https://docs.arduino.cc/tutorials/uno-r4-wifi/wifi-examples#wi-fi-web-client-ssl
*/


#include "WiFiS3.h"
#include "WiFiSSLClient.h"
#include "IPAddress.h"

#include "arduino_secrets.h"
#include <SPI.h>
#include <SD.h>

#include <dht11.h>
#define DHT11PIN 8

#include <Adafruit_BMP280.h>

Adafruit_BMP280  bmp; // I2C

dht11 DHT11;

File myFile;

int lastSecond = 0;


///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;        // your network password (use for WPA, or use as key for WEP)

int status = WL_IDLE_STATUS;
// if you don't want to use DNS (and reduce your sketch size)
// use the numeric IP instead of the name for the server:
IPAddress server(74,125,232,128);  // numeric IP

// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):
WiFiSSLClient client;

int myFile;
/* -------------------------------------------------------------------------- */
void setup() {
/* -------------------------------------------------------------------------- */
  //Initialize serial and wait for port to open:
  Serial.begin(115200);
  

  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    while (1);
  }

  myFile = SD.open("test.txt", FILE_WRITE);

  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }

    /* Default  settings from datasheet. */
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /*  Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp.  oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure  oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering.  */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */



    DigitalWrite(greenLedPin, HIGH);
                }

/* -------------------------------------------------------------------------- */
void loop() {
/* -------------------------------------------------------------------------- */
  
  // attempt to connect to WiFi network:
  if (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);

    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    unsigned long waiter = millis();
    while (millis() - waiter < 10000) {
        int x = 5
    }
   
  } else {
    printWifiStatus();

    Serial.println("\nStarting connection to server...");
    // if you get a connection, report back via serial:

    if (client.connect(server, 443)) {
        DigitalWrite(RedLedPin, HIGH);
        Serial.println("connected to server");
        // Connect to WPA/WPA2 network.
        status = WiFi.begin(ssid, pass);

        // wait 10 seconds for connection:
        unsigned long waiter = millis();
        while (millis() - waiter < 10000) {
            int x = 5
        } // Make a HTTP request:
        
        if (myFile) {
            
            myFile = SD.open("test.txt");
            while (myFile.available()) {
            client.println(myFile.read());
            }
                
        }
        
    }


  }


  if (second() != last_second) {
  //read stuff and print message to file
    Message newData;
    newData.time = millis()
    newData.light = analogueRead(A0)
    newData.pressure = float P = bmp.readPressure()/3386.39;
    newData.humidity = (float)DHT11.humidity
    newData.temp = (float)DHT11.temperature;

    myFile = SD.open("test.txt", FILE_WRITE);
    myFile.println(newData);
    last_second = second();
  }


}

/* just wrap the received data up to 80 columns in the serial print*/
/* -------------------------------------------------------------------------- */
void read_response() {
/* -------------------------------------------------------------------------- */
  uint32_t received_data_num = 0;
  while (client.available()) {
    /* actual data reception */
    char c = client.read();
    /* print data to serial port */
    Serial.print(c);
    /* wrap data to 80 columns*/
    received_data_num++;
    if(received_data_num % 80 == 0) {
      Serial.println();
    }
  }




read_response();

  // if the server's disconnected, stop the client:
  if (!client.connected()) {
    Serial.println();

  }
}

/* -------------------------------------------------------------------------- */
void printWifiStatus() {
/* -------------------------------------------------------------------------- */
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}