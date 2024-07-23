
#include <SoftwareSerial.h>
#include <Sabertooth.h>


// Up to 8 Sabertooth/SyRen motor drivers can share the same S1 line. //S1 goes to ESp32 TX
// This sample uses three: address 128 and 129 on ST1[0] and ST1[2],
// and address 130 on ST2.
SoftwareSerial SWSerial(NOT_A_PIN, 13); //RX, TX
Sabertooth ST1(128, SWSerial);



// Define encoder pins
const int encoderPinA = 2; // Connect Encoder Channel A to Arduino digital pin 2
const int encoderPinB = 4; // Connect Encoder Channel B to Arduino digital pin 3

// Define motor control pins
const int motorPinPWM = 20; // PWM pin to control motor speed

// Encoder variables
volatile long encoderCount = 0;
volatile int lastEncoderAState;
volatile int encoderAState;

// Desired position
const long targetPosition = 1000; // Adjust as needed

void setup() {
  // Initialize motor control pins
  pinMode(motorPinPWM, OUTPUT);
  
  // Initialize encoder pins
  pinMode(encoderPinA, INPUT_PULLUP);
  pinMode(encoderPinB, INPUT_PULLUP);
  
  // Attach interrupt for encoder Channel A
  attachInterrupt(digitalPinToInterrupt(encoderPinA), updateEncoder, CHANGE);
  
  // Set initial motor direction
  digitalWrite(motorPinDirection, HIGH); // Set motor direction (adjust based on your motor orientation)
}

void loop() {
  // Calculate error (difference between target and current position)
  long error = targetPosition - encoderCount;
  
  // Adjust motor speed based on error
  int motorSpeed = map(error, -1000, 1000, -255, 255); // Map error to motor speed range
  
  // Limit motor speed to avoid excessive changes
  motorSpeed = constrain(motorSpeed, -255, 255);
  
  // Set motor direction
  if (motorSpeed >= 0) {
    digitalWrite(motorPinDirection, HIGH); // Forward direction
  } else {
    digitalWrite(motorPinDirection, LOW); // Reverse direction
  }
  
  // Set motor speed
  analogWrite(motorPinPWM, abs(motorSpeed));
  
  delay(10); // Adjust delay as needed
}

// Interrupt Service Routine (ISR) for encoder
void updateEncoder() {
  encoderAState = digitalRead(encoderPinA);
  if ((lastEncoderAState == LOW) && (encoderAState == HIGH)) {
    if (digitalRead(encoderPinB) == LOW) {
      encoderCount++;
    } else {
      encoderCount--;
    }
  }
  lastEncoderAState = encoderAState;
}
