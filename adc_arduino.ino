int sensor_Pin1 = A0 , sensor_Pin2 = A1; 
int sensor_Value1= 0 , sensor_Value2 = 0 ;
void setup() {
  Serial.begin(115200);
  analogReference(INTERNAL);
  
}

void loop() {
    // read the value from the sensor:

    sensor_Value1 = analogRead(sensor_Pin1);
    Serial.println(sensor_Value1);
    sensor_Value2 = analogRead(sensor_Pin2);
    Serial.println(sensor_Value2);
    delay(400);
 
  }
  
