
// Software Serial Sample for Packet Serial
// Copyright (c) 2012 Dimension Engineering LLC
// See license.txt for license details.

#include <PS4Controller.h>
#include <SoftwareSerial.h>
#include <Sabertooth.h>


// Up to 8 Sabertooth/SyRen motor drivers can share the same S1 line. //S1 goes to ESp32 TX
// This sample uses three: address 128 and 129 on ST1[0] and ST1[2],
// and address 130 on ST2.
SoftwareSerial SWSerial(NOT_A_PIN, 25); //RX, TX
Sabertooth ST1(128, SWSerial);


long lastTimeStamp;

void onConnect(){
  Serial.println("Connected!.");
}

void onDisConnect(){
  Failsafe();
  Serial.println("Disconnected!.");    
}

void Failsafe(){
  Stop();
}

// Function to apply deadzone logic
int applyDeadzone(int value, int deadzone) {
  if (value > deadzone || value < -deadzone) {
    // Map the value to the range of 127 to -127
    return map(value, -127, 127, -30, 30);
    //return value;
  }
  return 0;  // Return 0 if within the deadzone
}



  void Forward(int speed1, int speed2){
    ST1.motor(1, speed1);
    ST1.motor(2 , speed2);
    Serial.println("F ");
    //Serial.println("both motor will turn forward");
  }
  void Backward(int speed1, int speed2){
    ST1.motor(1, speed1);
    ST1.motor(2,speed2);
    Serial.println("B ");
    //Serial.println("both motor will turn reverse");
  }
  void TurnRight(int speed1, int speed2){
    ST1.motor(2,speed1);
    ST1.motor(1,speed2);
    Serial.println("R");
    //Serial.println("bot will turn right");  
  }
  void TurnLeft(int speed1, int speed2){
    ST1.motor(2,speed1);
    ST1.motor(1,speed2);
    Serial.println("L");
    //Serial.println("bot will turn left");
  }

  void Stop(){
    ST1.motor(1,0);
    ST1.motor(2,0);
    //Serial.println("stop");
    // Serial.println("bot will stand still");
  }


void setup(){
  //Initialize Software Serial with Sabretooth AutoBaud
  SWSerial.begin(9600);
  Sabertooth::autobaud(SWSerial); // Autobaud is for the whole serial line -- you don't need to do
                                  // it for each individual motor driver. This is the version of
                                  // the autobaud command that is not tied to a particular
                                  // Sabertooth object.
                                  // See the examples in 1.Basics for information on whether you
                                  // need this line at all.

  //Initialize USB Serial
  Serial.begin(115200);

  //Attach Functions to Event Listeners
  //PS4.attach(responseToController);
  PS4.attachOnConnect(onConnect);
  PS4.attachOnDisconnect(onDisConnect);
  
  //Initialize PS4 Controller Master
  PS4.begin("a8:42:e3:8e:98:8c");

  Serial.println("Ready.");

  //Reset To Default State
  Failsafe();

}



void loop() {
  if (Serial.available() > 0) {
    String str = Serial.readStringUntil('\n'); // Read the string until newline character
    if (str.equals("batlvl")) { // Compare strings using equals() method
      Serial.printf("Battery Level : %d\n", PS4.Battery());
      Serial.println();
    }
  }

  int RightY = PS4.RStickY();
  int RightX = PS4.RStickX();
  int LeftY = PS4.LStickY();
  int LeftX = PS4.LStickX();

   //actuator
  if(applyDeadzone(RightY, 20) != 0){
    if(RightX > 0){
      Forward(RightY, RightY);
    }
    if(RightX < 0){
      Backward(RightY, RightY);
    }
  }else if(applyDeadzone(RightX, 20) != 0){
    if(RightX > 0){
      TurnRight(-RightX, RightX);
    }
    if(RightX < 0){
      TurnLeft(-RightX, RightX);
    }
  }else{
    Stop();
  }
  

  // Only needed to print the message properly on serial monitor. Else we don't need it.
  if (millis() - lastTimeStamp > 50) {
    lastTimeStamp = millis();
  }
}



