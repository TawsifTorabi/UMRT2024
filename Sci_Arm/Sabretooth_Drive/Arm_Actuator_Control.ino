#include <PS4Controller.h>
#include <SoftwareSerial.h>
#include <Sabertooth.h>


//C8:F0:9E:7A:C3:06 - Torabi ESP32 Mac 


// Up to 8 Sabertooth/SyRen motor drivers can share the same S1 line.
// This sample uses three: address 128 and 129 on ST1[0] and ST1[2],
// and address 130 on ST2.
SoftwareSerial SWSerial(NOT_A_PIN, 13);
Sabertooth ST1[2] = { Sabertooth(130, SWSerial), Sabertooth(129, SWSerial) };


long lastTimeStamp;


void onConnect(){
  Serial.println("Connected!.");
}

void onDisConnect(){
  Serial.println("Disconnected!.");    
}


// Function to apply deadzone logic
int applyDeadzone(int value, int deadzone) {
  if (value > deadzone || value < -deadzone) {
    // Map the value to the range of 127 to -127
    return map(value, -127, 127, -127, 127);
    //return value;
  }
  return 0;  // Return 0 if within the deadzone
}

void responseToController(){
  
  // PS4.LStickX(),
  // PS4.LStickY(),
  // PS4.RStickX(),
  // PS4.RStickY(),
  // PS4.Left(),
  // PS4.Down(),
  // PS4.Right(),
  // PS4.Up(),
  // PS4.Square(),
  // PS4.Cross(),
  // PS4.Circle(),
  // PS4.Triangle(),
  // PS4.L1(),
  // PS4.R1(),
  // PS4.L2(),
  // PS4.R2(),  
  // PS4.Share(),
  // PS4.Options(),
  // PS4.PSButton(),
  // PS4.Touchpad(),
}


void Actuator(int speed, int Actuator){
  if(Actuator == 0){
     ST1[1].motor(1, speed);
  }
  if(Actuator == 1){
     ST1[1].motor(2, speed);
  }
}

void Gripper(int speed, int Gripper){
  if(Gripper == 0){
     ST1[0].motor(1, speed);
  }
  if(Gripper == 1){
    ST1[1].motor(2, speed);
  }
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
  PS4.begin();

  Serial.println("Ready.");
}



void loop(){
 int RightY = PS4.RStickY();
  int RightX = PS4.RStickX();
  int LeftY = PS4.LStickY();

  if(applyDeadzone(RightY, 20) != 0){
    Actuator(RightY, 0);
    Serial.print("RY: ");
    Serial.println(RightY);
  }else{
    Actuator(0, 0);
  }

  if(applyDeadzone(RightX, 20) != 0){
    Actuator(RightX, 1);
    Serial.print("RX: ");
    Serial.println(RightX);
  }else{
    Actuator(0, 1);
  }
  
  if(PS4.L1()){
    Gripper(80, 0);
    Serial.println("Grip Close");
  }else{
    Gripper(0, 0);
  }
  if(PS4.R1() == 1){
    Gripper(-80, 0);
    Serial.println("Grip Open");
  }else{
    Gripper(0, 0);
  }


  //Only needed to print the message properly on serial monitor. Else we don't need it.
  if (millis() - lastTimeStamp > 50) {
    lastTimeStamp = millis();
  }
}

