#include <Servo.h>

Servo servo;
int angle = 90; // Declare global angle variable

void setup() {
  Serial.begin(9600);
  servo.attach(9); // Attach servo signal to pin 9
}

void loop() {
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n'); // Read the input string until newline character
    if (input.startsWith("angle-")) {
      angle = input.substring(6).toInt(); // Extract the integer part after "angle-" prefix
    }
  }
  angle = constrain(angle, 0, 180); // Constrain angle between 0 and 180 degrees
  int pulseWidth = map(angle, 0, 180, 1000, 2000); // Map angle to pulse width between 1000 and 2000 microseconds
  servo.writeMicroseconds(pulseWidth); // Set servo position
  Serial.println(angle);
  delay(100); // Delay to allow time for serial input buffer to clear
}
