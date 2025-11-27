/*

 This example is written for a network using WPA encryption. For
 WEP or WPA, change the WiFi.begin() call accordingly.

 Circuit:
 * Analog inputs attached to pins A0 through A5 (optional)


  Find the full UNO R4 WiFi Network documentation here:
  https://docs.arduino.cc/tutorials/uno-r4-wifi/wifi-examples#wi-fi-web-server
 */

#include <WiFiS3.h>
#include <DHT11.h>
#include <AGS02MA.h>
#include <TimeLib.h>
#include <Wire.h>

//BME280 pressureSensor;
DHT11 dht11(2);
AGS02MA AGS(26);

char serialSend[100];

int temp = 0;
int hum = 0;
int tvoc = 0;

unsigned long previousMillis = 0;
unsigned long previousJoystickMillis = 0;
long interval = 100;
 
int temporary = 0;

bool sendingMessage = false;

///////please enter your sensitive data in the Secret tab/arduino_secrets.h
//char ssid[] = "Linksys23718";        // your network SSID (name)
//char pass[] = "kmdjfy9qcn";       // your network password (use for WPA, or use as key for WEP)
char ssid[] = "Treyney WiFi"; 
char pass[] = "Bardo3434";
int keyIndex = 0;                 // your network key index number (needed only for WEP)

IPAddress ip(192,168,1,102);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);


int status = WL_IDLE_STATUS;


WiFiServer server(301);

void waiter() {
  unsigned long waiter = millis();
    while (millis() - waiter < 100) {
        int x = 5;
    }
}

void sendData() { 
  return;

}

time_t requestTimeSync()
{
  Serial.println("T:");  
  return 0; // the time will be sent later in response to serial mesg
}

void setup() {

  Serial.begin(9600);

  Wire.begin();

  dht11.setDelay(10);
    
  //pressureSensor.setI2CAddress(0x76);
  //if(pressureSensor.beginI2C() == false) Serial.println("EBarometric Pressure sensor connection failed");

  setSyncProvider(requestTimeSync); 



  //Initialize serial and wait for port to open:
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

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

}

int timesetter = 0;
void processSyncMessage() {
  unsigned long pctime;

  if(Serial.find('T')) {
    //setTime(int hr,int min,int sec,int day, int month, int yr);
     pctime = Serial.parseInt();
    setTime(pctime); // Sync Arduino clock to the time received on the serial port
    timesetter = 1;
  }
}




void gatherData() {

  unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= (interval*50)) {
        if(timeStatus() == timeSet) {
            time_t time = now();
            unsigned long seconds = (unsigned long) time;
            int result = dht11.readTemperatureHumidity(temp, hum);
            tvoc = AGS.readPPB();
            if(!sendingMessage) {
              if (result == 0) {
                  sprintf(serialSend, "Dtime=%lu;inTemp=%d;inHum=%d;tvoc=%d:", seconds, temp, hum, tvoc);
                  Serial.println(serialSend);
              } else {
                  // Print error message based on the error code.
                  Serial.println("ETemp/Humidity sensor connection failed");
              }
            }
        } else {
            requestTimeSync();
        }
        previousMillis = currentMillis;
    }

    if (Serial.available()) {
        processSyncMessage();
    } 


 sendData();
}





void Parser() {
  sendData();
}

void printWifiStatus() {
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


char message[100];
int index = 0;
int found = 0;

void loop() {
  // listen for incoming clients
  found = 0;
  if (status != WL_CONNECTED) {
    WiFi.config(ip, gateway, gateway, subnet);

  // attempt to connect to WiFi network:
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);
    Serial.println(status);
    // wait 10 seconds for connection:
    waiter();

  
    server.begin();
    Serial.println(WiFi.localIP());
    // you're connected now, so print out the status:
    printWifiStatus();
    gatherData();
    return;
  
  }

  WiFiClient client = server.available();
  if (client) {
    
    Serial.println("new client");
    while (client.connected()) {
      // if(client.available()) {
      //   char test[50];
      //   for(int i = 0; i < 50; i++) {
      //     test[i] = client.read();
      //   }
      //   char message[50] = "";
      //   sprintf(message, "%s", test);
      //   Serial.write(message);
      // }

      char c = ' ';
      int numBytes = client.available();
      index = 0;

      while (numBytes>0 && index < 50) {
        c = client.read();
        message[index] = c;
        
        index +=1;
        numBytes-=1;

        if (c == '\n') {
          found = 1;
          continue;
          
        }
        
      }

      if (index > 0 && found == 1) {
        message[index] = 0;
        sprintf(message, "%s", message);
        Serial.print(message);
        if(timesetter == 1) {
          char sendback[50];
          sprintf(sendback, "%ld:", now());
          client.print(sendback);
        }
        found == 0;
      }
      gatherData();
    }
    // give the web browser time to receive the data

    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  } else {
    gatherData();
  }

  //read data (client has example)

  //send to pi
}

