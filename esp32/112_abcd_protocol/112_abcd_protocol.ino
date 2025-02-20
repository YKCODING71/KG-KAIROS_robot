/*MIT License

Copyright (c) 2024 JD edu. http://jdedu.kr author: conner.jeong@gmail.com
     
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
     
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
     
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN TH
SOFTWARE.*/
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN              4           // DHT sensor signal pin   
#define DHTTYPE             DHT11       // put DHT sensor type (e.g.: DHT22 DHT11)

#define ECHOPIN             2           // Ultrasonic sensor echo pin
#define TRIGGER             8           // Ultrasonic sensor trigger pin 
#define SOUND_SPEED         0.034

#define LIGHT_A             A0          // Light sensor analog out 
#define LIGHT_D             5           // Light sensor digital out 

DHT_Unified dht(DHTPIN, DHTTYPE);

// Sensor data struct
struct sensor_data{
  float temperature;                    // temperature value
  float humidity;                       // humidity value 
  float distance;                       // distance value
  int light;                            // light sensor value
};

  
sensor_data sensor_val = { 0.0, 0.0, 0.0, 0};

void setup(){
  
  // Seria port set
  Serial.begin(115200);
  
  // setup for DHT sensor 
  dht.begin();
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  dht.humidity().getSensor(&sensor); 
  
  // Ultrasonic sensor pin setup
  pinMode(TRIGGER, OUTPUT);
  pinMode(ECHOPIN, INPUT);

  
}

float readUltraSensor(){
  float distance = 0;
  long duration = 0;
  // Clears the trigPin
  digitalWrite(TRIGGER, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(TRIGGER, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(ECHOPIN, HIGH);
  Serial.println(duration);
  // Calculate the distance
  distance = duration * SOUND_SPEED/2;
  
  return distance;  
}

void loop(){

  // get ultrasonic sensor value
  sensor_val.distance = readUltraSensor();

  // get temerature and humidity 
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature) == false) {
    sensor_val.temperature = event.temperature;
  }

  // Get humidity event and get its value.
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity) == false) {
    sensor_val.humidity = event.relative_humidity;
  }

  // Get light sensor analog value
  sensor_val.light = analogRead(LIGHT_A);

  // Send data to Raspberry pi 
  // protocol: a+"temperature"+b+"humidity"+c+"distance"+d+"light"+e
  Serial.print("a");
  Serial.print(sensor_val.temperature);
  Serial.print("b");
  Serial.print(sensor_val.humidity);
  Serial.print("c");
  Serial.print(sensor_val.distance);
  Serial.print("d");
  Serial.print(sensor_val.light);
  Serial.println("e");
  // To send packet to RPI properly, keep 500ms interval for loop().
  delay(500);
 
}