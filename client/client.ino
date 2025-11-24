/*
  TLS WiFi Web client

  Board CA Root certificate bundle is embedded inside WiFi firmware:
  https://github.com/arduino/uno-r4-wifi-usb-bridge/blob/main/certificates/cacrt_all.pem

  Find the full UNO R4 WiFi Network documentation here:
  https://docs.arduino.cc/tutorials/uno-r4-wifi/wifi-examples#wi-fi-web-client-ssl
*/

#include <SD.h>
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
char ssid[] = "Linksys23718";        // your network SSID (name)
char pass[] = "kmdjfy9qcn";     // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;                 // your network key index number (needed only for WEP)



int status = WL_IDLE_STATUS;
// if you don't want to use DNS (and reduce your sketch size)
// use the numeric IP instead of the name for the server:
IPAddress server(192,168,1,102);  // numeric IP


IPAddress ip(192,168,1,101);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);



// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):
WiFiClient client;


const int chipSelect = 10;
File myFile;


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

/* -------------------------------------------------------------------------- */
void setup() {
/* -------------------------------------------------------------------------- */
  //Initialize serial and wait for port to open:
  Serial.begin(9600);


  if (!SD.begin(chipSelect)) {
    digitalWrite(3, HIGH);
  }

  SD.remove("test.txt");
  

  

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

char message[100];
int indexEnd;
int out = 0;

void gatherData() {
  indexEnd = 0;


  unsigned long currentMillis = millis();


  sprintf(message, "hi;data;%d:", out);
  out +=1;
  //sprintf(message, "%ln;%ln;%ln:", data, data);
  Serial.println(message);




  write_to_sd();
}

void write_to_sd() {

  myFile = SD.open("test.txt", FILE_WRITE);
  myFile.println(message);
  myFile.close();
  return;
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

    unsigned long waiter = millis();
    while (millis() - waiter < 100) {
        int x = 5;
    }
   
  } else {
    //printWifiStatus();

    Serial.println("\nStarting connection to server...");
    // if you get a connection, report back via serial:

    Serial.println(counter);
    counter += 1;

    if (!client.connected())  {
      client.connect(server, 301);
    }
    if (client.connected()) {
      Serial.println("connected to server");
      myFile = SD.open("test.txt");
      if (myFile) {
        Serial.println("test.txt:");

        // read from the file until there's nothing else in it:
        while (myFile.available()) {
          client.write(myFile.read());
        }
        // close the file:
        myFile.close();
        SD.remove("test.txt");
      } else {
        // if the file didn't open, print an error:
        Serial.println("error opening test.txt");
      }

    }
        //read_response();
        // Connect to WPA/WPA2 network.

        // wait 10 seconds for connection:
             
  }

  
  gatherData();
        
  }



