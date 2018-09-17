
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Servo.h> 
 
Servo myservo_1 , myservo_2 , myservo_3;  // create servo object to control a servo              
int head_old = 0 , hand_old = 0   ;
String sensor1;
int head , hand;
const char *ssid = "Espm";
const char *password = "123456789";
ESP8266WebServer server(80);
void setup() {
      myservo_1.attach(0); // gpio 3 doesn't work when using wifi IO 3
    	myservo_2.attach(15); // D8
    	delay(1000);
    	Serial.begin(115200);
    	Serial.println();
    	Serial.print("Configuring access point...");
    	/* You can remove the password parameter if you want the AP to be open. */
    	WiFi.softAP(ssid, password);
    	IPAddress myIP = WiFi.softAPIP();
    	Serial.println(myIP);
    	server.on("/data/", HTTP_GET,  motor_movement);
    	server.begin();
    	Serial.println("HTTP server started");
}

void loop() {
	  server.handleClient();
 
}

void motor_movement() {                      
   //server.send(200, "text/html", "<form  method=\"POST\"> <input type=\"text\" name=\"motor_head\" placeholder=\"command_head\">  </br> <input type=\"text\" name=\"motor_hand\" placeholder=\"command_hand\">  </br>    <input type=\"submit\" value=\"send command\"> </form>  " );

  
  /*if(server.arg("motor_head") == "forward") {  //for application                               
    myservo_1.write(head_forward);              // tell servo to go to position in variable 'pos' 100 for head  40 for hand 
    server.send(200 , "text" , sensor1 );
    Serial.print("forward");                                         
  }
  else if(server.arg("motor_head") == "back")
  {
    myservo_1.write(head_back); // 140  head / 130 for hand
    Serial.print("back");  
  }
  
  else if( server.arg("motor_hand") == "close") { 
    myservo_2.write(hand_close);
    Serial.print("close");
  }
  else if( server.arg("motor_hand") == "open") { 
    myservo_2.write(hand_open);
    Serial.print("open");
  }
  */
          String head_degree = server.arg("head") ;
          String hand_degree = server.arg("hand") ;
          Serial.println(head_degree);
          Serial.println(hand_degree);
          hand = hand_degree.toInt();
          head = head_degree.toInt();
          if(head - head_old> 9 || head_old - head > 9 ){    // for removing vibrate
                head_old = head;
                head = 200 - head;  
                head = (double)head/2.40 + 70; 
                if(head <100) 
                  head=100;
                else if(head >150) 
                  head=150; 
                delay(100);
                myservo_2.write(head);
           
           }
           if( hand  - hand_old> 6 ||  hand_old- hand> 6 ){
                hand_old = hand;
                hand = (double)hand * 2.2 - 34  ;
                if(hand <48)
                   hand=25;
                else if(hand >130) 
                   hand=140;
                 delay(100);
                 myservo_1.write(hand);
           }
        
        
        
        
  
  
 }
