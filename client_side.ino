#include <ESP8266WiFi.h>
const char* ssid = "Espm";
const char* password = "123456789";

// Local ESP web-server address
const char *host = "192.168.4.1";
const int httpport = 80;
String url;
int counter = 0;
int head_degree , hand_degree;
void setup() {
  ESP.eraseConfig();
  WiFi.persistent(false);
  Serial.begin(115200);
  Serial.println();
  Serial.println("******** BEGIN ***********");
  delay(500);
  Serial.println("- set ESP STA mode");
  WiFi.mode(WIFI_STA);
  Serial.println("- connecting to wifi"); 
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
     delay(500);
     Serial.print(".");
  } 
  
  Serial.println("- wifi connected");
}

void sendHttpRequest() {
     WiFiClient client; 
     delay(100);
     if (!client.connect(host, httpport)) {
        Serial.println("connection failed");
        delay(500);
        return;
     } 
     client.print(String("GET ") + url + " HTTP/1.1\r\n" + "HOST: " + host + "\r\n"  +  "Connection: close\r\n\r\n");
}

void buildDataStream() { 
  if (Serial.available() > 3) {
       url = "/data/";
       url += "?head=";
       head_degree = Serial.parseInt();
       url += String(head_degree);
       url += "&hand=";
       hand_degree = Serial.parseInt();
       url += String(hand_degree);
       Serial.println("data stream: "+url);
  }
  
 
 /*
  data = "head=350&hand=200"; //test
  Serial.println(data);
  */
  
}



void loop() {    
    buildDataStream();
    if(WiFi.status() != WL_CONNECTED){
       Serial.println("disconnected");
    }
    sendHttpRequest();  
  }
