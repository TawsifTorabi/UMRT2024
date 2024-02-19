//Needle Servo Control with LED Indication

#include <Servo.h>

#define y_input A0

int LedRed = 10;
int LedGreen = 11;
int LedYellow = 12;
int yellowThreshold = 30; // Threshold to trigger the yellow LED

Servo servo;  // Create a servo object

void setup() {
  Serial.begin(9600);  // Initialize serial communication at 9600 baud rate
  servo.attach(9);     // Attach the servo to pin 9
  pinMode(y_input, INPUT);
  pinMode(LedRed, OUTPUT);
  pinMode(LedGreen, OUTPUT);
  pinMode(LedYellow, OUTPUT);
}

// Y
// max - 297
// min - 190

void loop() {
  if (Serial.available() > 0) {
    int angle = Serial.parseInt();
    if (angle >= 0 && angle <= 180) {
      servo.write(angle);
      Serial.print("Set angle to: ");
      Serial.println(angle);
    } else {
      Serial.println("Angle out of range (0-180). Please enter a valid angle.");
    }
    while (Serial.available() > 0) Serial.read();
  }

  int y_value = analogRead(y_input);

  y_value = map(y_value, 297, 190, 74, 180);
  servo.write(y_value);


  digitalWrite(LedRed, LOW);
  digitalWrite(LedGreen, LOW);
  digitalWrite(LedYellow, LOW);

  Serial.println(y_value);
}
