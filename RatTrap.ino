/* 
  Smart Rat Trap v1.0
  
  An Arduino-powered live rat trap that outsmarts clever rodents!
  
  More info at http://autodidacts.io/make-smart-rat-trap
  
  This code is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This code is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

/*  PIN ASSIGNMENTS  */
int sensorPin = 1;
 
int ledRedPin = 7;
int ledGreenPin = 5;
int ledBluePin = 6;

int buzzerPin = 8;
int catchPin = 12;

int threshold = 0;
int triggered = 0;

int recalibrationInterval = 10; 

long recalibrateTimer = (recalibrationInterval*60000);

void setup() {
  
  /* Un-comment serial commands for testing/debugging output */
  
  Serial.begin(9600);
  
  pinMode(13, OUTPUT);
  digitalWrite(13,LOW);
  
  pinMode(sensorPin, INPUT);
  
  pinMode(catchPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT); 
  pinMode(ledRedPin, OUTPUT); 
  pinMode(ledGreenPin, OUTPUT); 
  pinMode(ledBluePin, OUTPUT); 
  
  //Serial.println("Start");   
  
  auto_calibrate();
} 
  
void setLed(int pin){
  digitalWrite(ledGreenPin,LOW);
  digitalWrite(ledRedPin,LOW);
  digitalWrite(ledBluePin,LOW);
  if(pin != 0){
    digitalWrite(pin,HIGH);
  }
}

void soundAlarm(){
  for(int cycles = 0; cycles < 25; cycles++){
    for(int i = 0; i < 600; i++){  
      digitalWrite(buzzerPin, HIGH);
      delayMicroseconds(500);  
      digitalWrite(buzzerPin, LOW);
      delayMicroseconds(500);
     }
    delay(500);
  }
}

/* Sets the sensor threshold based on the highest of a series of readings */
void auto_calibrate(){ 
  //Serial.println("Calibrating...");
  int max_value = 0;
  int current_value = 0;
  for(int i = 0; i < 10; i++){
    current_value = analogRead(sensorPin); 
    //Serial.println(current_value);
    if(current_value > max_value){
       max_value = current_value;
    }
    setLed(ledGreenPin);
    delay(300);
    setLed(0);
    delay(300);    
  }
  threshold = max_value+5;
  //Serial.print("Auto calibrated threshold:");
  //Serial.println(threshold);
  setLed(ledGreenPin);
}


void loop() {   

  
  if(triggered == 0){ 

    if(millis() > recalibrateTimer){
      //Serial.print("Recalibrating...");
      auto_calibrate();
      recalibrateTimer = (millis() + (recalibrationInterval*60000));
    }    
    
    if(analogRead(sensorPin) > threshold){
      //Serial.print(analogRead(sensorPin));
      //Serial.println(" - threshold crossed");
      delay(10);
      setLed(ledRedPin);
      if(analogRead(sensorPin) > threshold){ 
         digitalWrite(catchPin, HIGH);
         delay(200);
         digitalWrite(catchPin, LOW); 
         triggered = 1;
         setLed(ledBluePin); 
         soundAlarm();       
      }else{
        setLed(ledGreenPin);       
      }      
    }
  }
  delay(10);
}
