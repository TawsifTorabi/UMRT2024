#include <PS4Controller.h>
#include <SoftwareSerial.h>
#include <Sabertooth.h>

#define WRIST_X_PIN 14
#define WRIST_Z_PIN 12

SoftwareSerial SWSerial(NOT_A_PIN, 13);
Sabertooth ST1[2] = { Sabertooth(130, SWSerial), Sabertooth(129, SWSerial) };

void onConnect(){
  Serial.println("Connected!.");
}

void onDisConnect(){
  Failsafe();
  Serial.println("Disconnected!.");    
}

void setup(){
  SWSerial.begin(9600);
  Sabertooth::autobaud(SWSerial);
  Serial.begin(115200);
  
  PS4.attachOnConnect(onConnect);
  PS4.attachOnDisconnect(onDisConnect);
  PS4.begin();

  pinMode(WRIST_X_PIN, OUTPUT);
  pinMode(WRIST_Z_PIN, OUTPUT);

  Serial.println("Ready.");
}

void loop(){
  int RightY = applyDeadzone(PS4.RStickY(), 20);
  int RightX = applyDeadzone(PS4.RStickX(), 20);
  int LeftY = applyDeadzone(PS4.LStickY(), 20);

  // Actuator 1
  if(RightY != 0){
    Actuator(RightY, 0);
    Serial.print("RY: ");
    Serial.println(RightY);
  } else {
    Actuator(0, 0);
  }

  // Actuator 2
  if(RightX != 0){
    Actuator(RightX, 1);
    Serial.print("RX: ");
    Serial.println(RightX);
  } else {
    Actuator(0, 1);
  }
  
  // Gripper
  if(PS4.L1()){
    Gripper(80, 0);
    Serial.println("Grip Close");
  } else if(PS4.R1() == 1){
    Gripper(-80, 0);
    Serial.println("Grip Open");
  } else {
    Gripper(0, 0);
  }

  // Wrist Joint
  if(PS4.Up()){
    digitalWrite(WRIST_X_PIN, HIGH);
    Serial.println("WristX UP");
  } else if(PS4.Down()){
    digitalWrite(WRIST_X_PIN, LOW);
    Serial.println("WristX DOWN");
  } else if(PS4.Left()){
    digitalWrite(WRIST_Z_PIN, HIGH);
    Serial.println("WristZ UP");
  } else if(PS4.Right()){
    digitalWrite(WRIST_Z_PIN, LOW);
    Serial.println("WristZ DOWN");
  } else {
    digitalWrite(WRIST_X_PIN, LOW);
    digitalWrite(WRIST_Z_PIN, LOW);
  }

  // Joint Failsafe
  if (!PS4.Up() && !PS4.Down() && !PS4.Left() && !PS4.Right()) {
    jointFailsafe();
  }
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

void jointFailsafe(){
  digitalWrite(WRIST_X_PIN, LOW);
  digitalWrite(WRIST_Z_PIN, LOW);
}

int applyDeadzone(int value, int deadzone) {
  if (value > deadzone || value < -deadzone) {
    return map(value, -127, 127, -127, 127);
  }
  return 0;  // Return 0 if within the deadzone
}
