#include <ESP8266WiFi.h>

const char* ssid = "........";
const char* password = "........";
const char* host = "........";
const char* useragent = "........";

WiFiClient client;



void httpRequest()
{
  if (client.connect(host, 80)) {
    client.println("GET " + url + " HTTP/1.1");
    client.println("User-Agent: " + useragent);
    client.println("Connection: close");
    client.println();
    client.stop();
  }
}


void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.print("connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

}

void loop() {

}