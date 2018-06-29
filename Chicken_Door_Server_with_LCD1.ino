/*
 *    http://192.168.1.21/Open will indicate chicken door Open,
 *    http://192.168.1.21/Closed will indicate chicken door Closed.
 */
 
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const char* ssid = "**********";
const char* password = "***********";
const uint16_t port = 80;

const int builtInLed = D4;

IPAddress ip (192, 168, 1, 21);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

int val;

LiquidCrystal_I2C lcd(0x3F,16,2);

Servo myservo;

WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  delay(10);

  myservo.attach(D7);

  pinMode(builtInLed, OUTPUT);

  lcd.init();   
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("  WEMOS Server");
  delay (2000);
  lcd.setCursor(0,1);
  lcd.print("Initializing...");
  delay (5000);
  lcd.clear();
  
   // Connect to WiFi network
  Serial.println();
  Serial.println();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Connecting to...");
  delay(1000);
  Serial.print("Connecting to ");
  lcd.setCursor(0,1);
  lcd.print("   ** Wemos **");
  Serial.println(ssid);
  delay(5000);
  lcd.clear();

  WiFi.config(ip, gateway, subnet);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
     Serial.print(".");
     delay(500);
  }
  Serial.println("");
  Serial.println("WiFi connected");
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(" WiFi Connected");
  delay(2000);
  lcd.setCursor(0,1);
  lcd.print(" Server Started");
  delay(5000);
  lcd.clear();
  lcd.noBacklight();
  
  // Print the IP address
  server.begin();
  Serial.print("Server Started @ ");
  IPAddress ip = WiFi.softAPIP();
  Serial.println(WiFi.localIP());
     
}

void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  
  // Wait until the client sends some data
  Serial.println("new client");
    while(!client.available()){
    delay(1);
  }
  
  // Read the first line of the request
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();
  
  // Match the request
 
  if (req.indexOf("/Open") != -1)
    val = 0;
  else if (req.indexOf("/Closed") != -1)
    val = 1;        
  else {
    Serial.println("invalid request");
    client.stop();
    return;
  }
   // Set GPIO2 according to the request
  digitalWrite(builtInLed, val);
{
  if (val == LOW)
{    
      myservo.write(90);
      lcd.backlight();
      lcd.clear();
      delay(1000);
      lcd.setCursor(0,0);
      lcd.print("The Chickens are");
      delay(3000);
      lcd.setCursor(0,1);
      lcd.print("  in the Garden");
      delay(5000);
  }
    else
{
      myservo.write(0);
      lcd.backlight();
      lcd.clear();
      delay(1000);
      lcd.setCursor(0,0);
      lcd.print("  The Chickens");
      delay(3000);
      lcd.setCursor(0,1);
      lcd.print("have gone to bed");
      delay(5000);
  }
}
  
  client.flush();

  // Prepare the response
  String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\nThe Chicken Shed Door is ";
  s += (val)?"Open":"Closed";
  s += "</html>\n";

  // Send the response to the client
  client.print(s);
  delay(1);
  Serial.println("Client disonnected");
  
  // The client will actually be disconnected 
  // when the function returns and 'client' object is detroyed
}  
