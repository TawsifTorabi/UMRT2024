/*
Add a jumper on D4 for touch switch
this code can toggle the direction of the M3508 P16 motor with PWM using c620 ESC.
*/

#include <ESP32Servo.h>

Servo myservo;
int servoPin = 13;
int potPin = 34;
int ADC_Max = 4096;
int val;
int touchPin = 4;
int direction = 0;
int minRange = 0;
int maxRange = 90;
bool touchFlag = false;

void setup() {
  Serial.begin(115200);
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  myservo.setPeriodHertz(50);
  myservo.attach(servoPin, 50, 5000);
  pinMode(2, OUTPUT);
}

void loop() {
  val = analogRead(potPin);
  val = map(val, 0, ADC_Max, minRange, maxRange);
  myservo.write(val);
  Serial.println(val);
  delay(100);

  if (touchRead(touchPin) < 30 && !touchFlag) {
    touchFlag = true;
    if (digitalRead(2) == LOW) {
      digitalWrite(2, HIGH);
      direction = 1;
      minRange = 90;  
      maxRange = 179;
    } else {
      digitalWrite(2, LOW);
      direction = 0;
      minRange = 0;   
      maxRange = 90;
    }
  } else if (touchRead(touchPin) >= 30) {
    touchFlag = false;
  }
}
