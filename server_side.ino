
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Servo.h> 
 
Servo myservo_1 , myservo_2 , myservo_3 , myservo_4;  // create servo object to control a servo 
                // twelve servo objects can be created on most boards
int head_old =0 , hand_old =0  , pitch_old = 0  ; // you can use substring or string remove
int head , hand , pitch ;
/* Set these to your desired credentials. */
const char *ssid = "Espm";
const char *password = "123456789";

ESP8266WebServer server(80);



void setup() {
  ESP.eraseConfig();
  myservo_1.attach(0); // gpio 3 doesn't work when using wifi IO 3 right
  myservo_4.attach(16); //  IO 0 // hand left
  myservo_2.attach(15); // D8 //head 
  myservo_3.attach(5); // D1 // body
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
          String pitch_degree = server.arg("pitch");
          Serial.println(head_degree);
          Serial.println(hand_degree);
          Serial.println(pitch_degree);
          hand = hand_degree.toInt();
          head = head_degree.toInt();
          pitch = pitch_degree.toInt();
          
          if(pitch > 4 ) // right
          { 
              myservo_3.write(150); 
          }
          else if( pitch < -4){ // left
              myservo_3.write(60);
          }

          else   // mid
           { 
              myservo_3.write(100);
           }  
             

          
          if(head - head_old> 10 || head_old - head > 10 ){    // to remove vibration
                head_old = head; 
                head = (double)head; 
                if(head <170) 
                  head=160;
                else if(head >170) 
                  head=100; 
                delay(100);
                myservo_2.write(head);
           
             }
          if(hand- hand_old> 5 ||  hand_old- hand> 5 ){
                hand_old = hand;
                hand = (double)hand ;
                if(hand <350)
                   hand=110;
                else if(hand >400) 
                   hand=5;
                else
                  hand = 60;
                 delay(100);
                 myservo_1.write(hand);
                 myservo_4.write(130 -  hand);
           }

          
        
        
        
        
  
  
 }
