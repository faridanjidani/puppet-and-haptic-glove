#include <Filters.h>

int sensor_Pin1 = A0 , sensor_Pin2 = A1; 
int sensor_Value1= 0 , sensor_Value2 = 0 ;
// select the input pin for the potentiometer
float filterFrequency = 0.25; 
FilterOnePole lowpassFilter1( LOWPASS, filterFrequency ); 
FilterOnePole lowpassFilter2( LOWPASS, filterFrequency );  
void setup() {
  Serial.begin(115200);
  analogReference(INTERNAL);
  
}

void loop() {
  // read the value from the sensor:

   sensor_Value1 = analogRead(sensor_Pin1);
   sensor_Value1 = lowpassFilter1.input(sensor_Value1 );
   
   sensor_Value2 = analogRead(sensor_Pin2);
   sensor_Value2 = lowpassFilter2.input(sensor_Value2 );
   Serial.println(sensor_Value1);
   Serial.println(sensor_Value2);
   delay(300);
 
  }
  
