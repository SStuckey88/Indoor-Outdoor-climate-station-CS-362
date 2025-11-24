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
#include <SparkFunBME280.h>

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



///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = "iPhone";        // your network SSID (name)
char pass[] = "password362";    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;                 // your network key index number (needed only for WEP)

IPAddress staticIP(192, 168, 1, 100);


int status = WL_IDLE_STATUS;


WiFiServer server(80);

void waiter() {
  unsigned long waiter = millis();
    while (millis() - waiter < 100) {
        int x = 5;
    }
}

void sendData() { 
  return;

}

void setup() {

  Serial.begin(9600);

  Wire.begin();

  dht11.setDelay(10);
    
  pressureSensor.setI2CAddress(0x76);
  if(pressureSensor.beginI2C() == false) Serial.println("EBarometric Pressure sensor connection failed");

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
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }

  WiFi.config(staticIP);

  // attempt to connect to WiFi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);
    Serial.println(status);
    // wait 10 seconds for connection:
    waiter();

  }
   WiFi.config(staticIP);
  server.begin();
  Serial.println(WiFi.localIP());
  // you're connected now, so print out the status:
  printWifiStatus();
}





void gatherData() {

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

void Parser() {
  sendData();
}



void loop() {
  // listen for incoming clients
  WiFiClient client = server.available();
  if (client) {
    Serial.println("new client");
    while (client.connected()) {
      while (client.available()) {
        char c = client.read();
        Parser();
        waiter();
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