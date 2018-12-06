#include <ESP8266WiFi.h>

#include "Wire.h" 
// I2Cdev and MPU9150 must be installed as libraries, or else the .cpp/.h files
// for both classes must be in the include path of your project
#include "I2Cdev.h"
#include "MPU6050.h"
#include "helper_3dmath.h"

// class default I2C address is 0x68
// specific I2C addresses may be passed as a parameter here
// AD0 low = 0x68 (default for InvenSense evaluation board)
// AD0 high = 0x69
MPU6050 accelGyroMag;

int16_t ax, ay, az;
int16_t gx, gy, gz;
float roll =0  , pitch =0;

#define ACCELEROMETER_SENSITIVITY 8192.0
#define GYROSCOPE_SENSITIVITY 80.536
 
#define M_PI 3.14159265359      
#define dt 0.015

// AP Wi-Fi credentials
const char* ssid = "Espm";
const char* password = "123456789";

// Local ESP web-server address
const char *host = "192.168.4.1";
const int httpport = 80;
String url;
int counter = 0;
int head_degree , hand_degree;
const int MPU=0x53;  // I2C address was found by I2c scanner
int16_t AcX,AcY,AcZ;
//Static network configuration
/*IPAddress ip(192, 168, 4, 1);
IPAddress gateway(192, 168, 4, 1);
IPAddress subnet(255, 255, 255, 0);
WiFiClient client;
*/
void Gyro_setup(){
   Wire.begin(12, 13);
   Serial.println("Initializing I2C devices...");
   accelGyroMag.initialize();
   Serial.println("Testing device connections...");
   Serial.println(accelGyroMag.testConnection() ? "MPU9150 connection successful" : "MPU9150 connection failed");
   
}

void Wifi_setup(){
  WiFi.persistent(false);
  Serial.begin(115200);
  Serial.println();
  Serial.println("******** BEGIN ***********");
  delay(500);
  Serial.println("- set ESP STA mode");
  WiFi.mode(WIFI_STA);
  Serial.println("- connecting to wifi");
 // WiFi.config(ip, gateway, subnet); 
  WiFi.begin(ssid, password);      
  while (WiFi.status() != WL_CONNECTED) {
     delay(500);
     Serial.print(".");
  } 
  Serial.println("- wifi connected");
}

void ComplementaryFilter(int16_t gx, int16_t gy ,int16_t gz , int16_t ax, int16_t ay ,int16_t az  , float &pitch, float &roll)
{
    float pitchAcc, rollAcc;               
 
    // Integrate the gyroscope data -> int(angularSpeed) = angle
    pitch += ((float)gx / GYROSCOPE_SENSITIVITY) * dt; // Angle around the X-axis
    roll -= ((float)gy / GYROSCOPE_SENSITIVITY) * dt;    // Angle around the Y-axis
 
    // Compensate for drift with accelerometer data if !bullshit
    // Sensitivity = -2 to 2 G at 16Bit -> 2G = 32768 && 0.5G = 8192
    int forceMagnitudeApprox = abs(ax) + abs(ay) + abs(az);
    if (forceMagnitudeApprox > 6192 && forceMagnitudeApprox < 32768)
    {
        // Turning around the X axis results in a vector on the Y-axis
        pitchAcc = atan2f((float)ay, (float)az) * 180 / M_PI;
        pitch = pitch * 0.97 + pitchAcc * 0.03;
 
        // Turning around the Y axis results in a vector on the X-axis
        rollAcc = atan2f((float)ax, (float)az) * 180 / M_PI;
        roll = roll * 0.97 + rollAcc * 0.03;
    }

    if(pitch > 8 )
      pitch =8;
    else if (pitch <-8)
      pitch = -8;
    
} 


void setup() {
  ESP.eraseConfig();
  Gyro_setup();
  Wifi_setup();
}

void sendHttpRequest() {
     WiFiClient client; 
     //delay(50);
     if (!client.connect(host, httpport)) {
        Serial.println("connection failed");
        
        delay(500);
        return;
     } 
     client.print(String("GET ") + url + " HTTP/1.1\r\n" + "HOST: " + host + "\r\n"  +  "Connection: close\r\n\r\n");
}

bool buildDataStream() { 
  if (Serial.available() > 3) {
       accelGyroMag.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
       ComplementaryFilter( gx,  gy , gz , ax, ay ,az ,  pitch,  roll);
       url = "/data/";
       url += "?head=";
       head_degree = Serial.parseInt();
       url += String(head_degree);
       url += "&hand=";
       hand_degree = Serial.parseInt();
       url += String(hand_degree);
       url += "&pitch=";
       url += String(pitch);
       url += "&roll=";
       url += String(roll);
       Serial.println("data stream: "+url);
       return 0;
  }
  else 
      return 1;
  
 
 /*
  data = "head=350&hand=200"; //test
  Serial.println(data);
  */
  
}


void loop() {
    bool flag = buildDataStream();
    if(WiFi.status() != WL_CONNECTED){
       Serial.println("disconnected");
    }
    if( !flag ) 
      sendHttpRequest();
      //ESP.reset();
  }
