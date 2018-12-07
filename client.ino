#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#define N_SENSOR 13
#define N_LED 12

const char* ssid = "........";
const char* password = ".......";

volatile int sys_status = 0; // 0 - регестрирует срабатывания; 1 - сработал; -1 - не регестрирует срабатывания
volatile int val = 0;

ESP8266WebServer server(80);

void http_get_status() {
  
  String message = "Sensor status: ";
  
  if (sys_status == -1) {message += "Not working\n";}
  else if (sys_status == 1) {message += "Triggered\n";}
  else if (sys_status == 0) {message += "Working\n";}
  
  
  server.send(200, "text/plain", message);
}

void http_sensor_on()
{
  String message = "Sensor status: Working\n";
  sys_status = 0;
  
  server.send(200, "text/plain", message);
}

void http_sensor_off()
{
  String message = "Sensor status: Not working\n";
  sys_status = -1;
  digitalWrite(N_LED, LOW);
  
  server.send(200, "text/plain", message);
}

void http_notification_off()
{
  String message = "Sensor status: ";
  if ((sys_status == 1) || (sys_status == 0))  {sys_status = 0; message += "Working\n"; digitalWrite(N_LED, LOW);}
  if (sys_status == -1) {message += "Not working\n";}

  server.send(200, "text/plain", message);
}

void handleNotFound() {
  String message = "Shit happens\n";
  server.send(404, "text/plain", message);
}


void handleInterrupt()
{
  if (sys_status == 0)
    {
      sys_status = 1;
      digitalWrite(N_LED, HIGH);
    }
}

void setup(void) {
  pinMode(N_SENSOR, INPUT);
  attachInterrupt(digitalPinToInterrupt(N_SENSOR), handleInterrupt, RISING);
  pinMode(N_LED, OUTPUT);
  digitalWrite(N_LED, LOW);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/get_status", http_get_status);
  server.on("/notification_off", http_notification_off);
  server.on("/sensor_on", http_sensor_on);
  server.on("/sensor_off", http_sensor_off);

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
 
  server.handleClient();
  
}
