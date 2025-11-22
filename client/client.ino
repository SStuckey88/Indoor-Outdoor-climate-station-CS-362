/*
  TLS WiFi Web client

  Board CA Root certificate bundle is embedded inside WiFi firmware:
  https://github.com/arduino/uno-r4-wifi-usb-bridge/blob/main/certificates/cacrt_all.pem

  Find the full UNO R4 WiFi Network documentation here:
  https://docs.arduino.cc/tutorials/uno-r4-wifi/wifi-examples#wi-fi-web-client-ssl
*/


#include <WiFiS3.h>
#include <DHT11.h>
#include <AGS02MA.h>
#include <TimeLib.h>
#include <Wire.h>
#include <SparkFunBME280.h>

BME280 pressureSensor;
DHT11 dht11(2);
AGS02MA AGS(26);


int lastSecond = 0;

char serialSend[100];

int temp = 0;
int hum = 0;
int tvoc = 0;
float barPres = 0.0;


///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = "Maid Café";        // your network SSID (name)
char pass[] = "Sysadmin24!";     // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;                 // your network key index number (needed only for WEP)



int status = WL_IDLE_STATUS;
// if you don't want to use DNS (and reduce your sketch size)
// use the numeric IP instead of the name for the server:
IPAddress server(172, 25, 6, 50);  // numeric IP

IPAddress ip(172, 25, 6, 51);
IPAddress gateway(172,25,1,82);
IPAddress subnet(255,255,255,0);



// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):
WiFiSSLClient client;



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

int myFile;
/* -------------------------------------------------------------------------- */
void setup() {
/* -------------------------------------------------------------------------- */
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  

  /*if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    while (1);
  }

  myFile = SD.open("test.txt", FILE_WRITE);*/

  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  Serial.println(fv);
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }

    /* Default  settings from datasheet. 



    DigitalWrite(greenLedPin, HIGH);*/
                }



void sendData() { 
  return;

}

void gatherData() {

  unsigned long currentMillis = millis();
  //Read Joystick data every 100ms - send to Pi if changed since last read
    /*
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
        } else {
            requestTimeSync();
        }
        previousMillis = currentMillis;
    }

    if (Serial.available()) {
        processSyncMessage();
    } **/


 sendData();
}

int counter = 0;



/* -------------------------------------------------------------------------- */
void loop() {
/* -------------------------------------------------------------------------- */
  
  // attempt to connect to WiFi network:
  if (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);

    WiFi.config(ip, gateway, gateway, subnet); 

    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    unsigned long waiter = millis();
    while (millis() - waiter < 10000) {
        int x = 5;
    }
   
  } else {
    //printWifiStatus();

    Serial.println("\nStarting connection to server...");
    // if you get a connection, report back via serial:

    Serial.println(counter);
    counter += 1;
    if (client.connect(server, 301)) {
        Serial.println("connected to server");
        client.println("hi");
        //read_response();
        // Connect to WPA/WPA2 network.

        // wait 10 seconds for connection:
        } 
        
        /*if (myFile) {
            
            myFile = SD.open("test.txt");
            while (myFile.available()) {
            client.println(myFile.read());
            }*/
                
        }
        
    }



