
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>

const char* ssid = "*********";
const char* password = "***********";

const int DoorGpioPin = D2;
const int ledGpioPin =D4;
const int pinLed1 = D5;
const int pinLed2 = D6;

int doorState=0;
int ledState=0;
int val = 0;

#define closed 0
#define opened 1

const char* host = "192.168.1.21";
const uint16_t port = 80;

void setup() {
Serial.begin(115200);
delay(10);

pinMode(DoorGpioPin, INPUT_PULLUP);
pinMode(pinLed1, OUTPUT);
pinMode(pinLed2, OUTPUT);
pinMode(ledGpioPin, OUTPUT);

// Connect to WiFi network
Serial.println();
Serial.println();
Serial.print("Connecting to... ");
Serial.println(ssid);

IPAddress ip (192, 168, 1, 20);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);
  
WiFi.config(ip, gateway, subnet);
WiFi.begin(ssid, password);

while (WiFi.status() != WL_CONNECTED) {
delay(500);
Serial.print(".");
}

Serial.println("");
Serial.println("WiFi connected");

// Start the server
Serial.print("Chicken Shed Online @ ");

// Print the IP address
Serial.println(ip);

}
void loop() {

val = digitalRead(DoorGpioPin);

if (val==LOW && doorState==closed){ // Print button pressed message.
SendDoorClosedNotification();
Serial.println("Door Closed");
digitalWrite(pinLed1, LOW);
digitalWrite(pinLed2, HIGH);
doorState=opened;
ledState=1;//on
digitalWrite(ledGpioPin, ledState);
}
yield();
if (val==HIGH && doorState==opened){
SendDoorOpenNotification();
Serial.println("Door Open");
digitalWrite(pinLed1, HIGH);
digitalWrite(pinLed2, LOW);
doorState=closed;
ledState=0; //off
digitalWrite(ledGpioPin, ledState);
}
yield(); // this is most important part of the code, as it tells the esp8266 keep running background wifi work,
//without this, your code will disconnect from wifi, after long run of code.
}

void SendDoorClosedNotification()
{
    Serial.print("Connecting to Server @ ");
    Serial.println(host); 
    
WiFiClient client;

if (!client.connect(host, port)) {
        Serial.println("Connection FAILED");
        delay(500);
        return;
}

String url = "/http://192.168.1.21/Closed";

Serial.print("Requesting URL: ");
Serial.println(url);

     // Make a HTTP request to the server
     client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                  "Host: " + host + "\r\n" +
                  "Connection: close\r\n\r\n"); 
     
     delay(10);

   // Read all the lines of the reply from server and print them to Serial
    Serial.println("Respond:");
    while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);

}

Serial.println();
Serial.println("Closing Connection");
}

void SendDoorOpenNotification()
{
    Serial.print("Connecting to Server @ ");
    Serial.println(host); 
    
WiFiClient client;

if (!client.connect(host, port)) {
        Serial.println("Connection FAILED");
        delay(500);
        return;
}

String url2 = "/http://192.168.1.21/Open";

Serial.print("Requesting URL: ");
Serial.println(url2);

     // Make a HTTP request to the server
     client.print(String("GET ") + url2 + " HTTP/1.1\r\n" +
                  "Host: " + host + "\r\n" +
                  "Connection: close\r\n\r\n"); 
     
     delay(10);

   // Read all the lines of the reply from server and print them to Serial
    Serial.println("Respond:");
    while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);

}

Serial.println();
Serial.println("Closing Connection");
}
