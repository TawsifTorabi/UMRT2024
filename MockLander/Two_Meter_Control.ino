//Two Meter Control with Two servo paired to two axis and with LED Indicator
#include <Servo.h>

#define y_input A0
#define x_input A1

int LedRed1 = 10;
int LedGreen1 = 11;
int LedYellow1 = 12;

int LedRed2 = 4;
int LedGreen2 = 3;
int LedYellow2 = 2;

int yellowThreshold = 30; // Threshold to trigger the yellow LED

Servo servo1;  // Create a servo object for first servo
Servo servo2;  // Create a servo object for second servo

int currentAngle1 = 90; // Initial angle for the first servo
int currentAngle2 = 90; // Initial angle for the second servo

void setup() {
  Serial.begin(9600);  // Initialize serial communication at 9600 baud rate
  servo1.attach(9);     // Attach the first servo to pin 9
  servo2.attach(5);     // Attach the second servo to pin 5
  
  pinMode(y_input, INPUT);
  pinMode(x_input, INPUT);
  
  pinMode(LedRed1, OUTPUT);
  pinMode(LedGreen1, OUTPUT);
  pinMode(LedYellow1, OUTPUT);
  
  pinMode(LedRed2, OUTPUT);
  pinMode(LedGreen2, OUTPUT);
  pinMode(LedYellow2, OUTPUT);
}

void loop() {
  digitalWrite(LedRed1, LOW);
  digitalWrite(LedGreen1, LOW);
  digitalWrite(LedYellow1, LOW);
  
  digitalWrite(LedRed2, LOW);
  digitalWrite(LedGreen2, LOW);
  digitalWrite(LedYellow2, LOW);

  int y_value = analogRead(y_input);
  int x_value = analogRead(x_input);

  int mappedAngle1 = map(y_value, 297, 190, 74, 180); // Map the y_value to servo angle range for first servo
  int mappedAngle2 = map(x_value, 100, 220, 74, 180); // Map the x_value to servo angle range for second servo
  
  moveServo(servo1, currentAngle1, mappedAngle1, LedRed1, LedGreen1, LedYellow1);
  moveServo(servo2, currentAngle2, mappedAngle2, LedRed2, LedGreen2, LedYellow2);
  
  Serial.print("Mapped Angle 1: ");
  Serial.println(mappedAngle1);
  Serial.print("Mapped Angle 2: ");
  Serial.println(mappedAngle2);
}

void moveServo(Servo servo, int &currentAngle, int targetAngle, int ledRed, int ledGreen, int ledYellow) {
  if(targetAngle >= 74 && targetAngle <= 180) {
    if(targetAngle > currentAngle) {
      for(int i = currentAngle; i <= targetAngle; i++) {
        servo.write(i);
        updateLEDs(i, ledRed, ledGreen, ledYellow);
        delay(30); // Adjust the delay as needed
      }
    } else {
      for(int i = currentAngle; i >= targetAngle; i--) {
        servo.write(i);
        updateLEDs(i, ledRed, ledGreen, ledYellow);
        delay(20); // Adjust the delay as needed
      }
    }
    currentAngle = targetAngle; // Update current angle
  }
}

void updateLEDs(int servoAngle, int ledRed, int ledGreen, int ledYellow) {
  if (servoAngle <= 175) {
    digitalWrite(ledYellow, HIGH);
    if (servoAngle <= 120) {
      digitalWrite(ledGreen, HIGH);
      if (servoAngle <= 74) {
        digitalWrite(ledRed, HIGH);
      }
    }
  }
}
