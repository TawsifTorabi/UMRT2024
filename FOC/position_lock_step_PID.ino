//Using DFRobot Encoded Zen Motor
#include <PIDController.h>

volatile long int encoder_pos = 0;

PIDController pos_pid; 
int motor_value = 255;
unsigned int integerValue=0;  // Max value is 65535
char incomingByte;

int encoderpinA = 2;
int encoderpinB = 4;

int en = 9;
int inA = 10;
int inB = 11;


//Declared Constants
const int motorStepsPerTurn = 32;
const float gearRatio = 43.8;

//Variables
volatile int encoderSteps = 0;
volatile float motorTurns = 0.0;
volatile float shaftTurns = 0.0;

void setup() {

  Serial.begin(9600);

  pinMode(encoderpinA, INPUT);
  pinMode(encoderpinB, INPUT);

  pinMode(en, OUTPUT);
  pinMode(inA, OUTPUT);
  pinMode(inB, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(encoderpinA), encoder, RISING);

  pos_pid.begin();    
  pos_pid.tune(15, 0, 2000);    //kp,ki,kd
  //pos_pid.tune(15, 0, 2000);    //kp,ki,kd
  pos_pid.limit(-255, 255);
}

void loop() {

  if (Serial.available() > 0) {  
      integerValue = 0;        
      while(1) {           
        incomingByte = Serial.read();
        if (incomingByte == '\n') break;   
        if (incomingByte == -1) continue;  
        integerValue *= 10;  
        integerValue = ((incomingByte - 48) + integerValue);
        pos_pid.setpoint(integerValue);
      }
  }

  motor_value = pos_pid.compute(encoder_pos);
  if(motor_value > 0){
    MotorCounterClockwise(motor_value);
  }else{
    MotorClockwise(abs(motor_value));
  }

  motorTurns = float(encoder_pos) / float(motorStepsPerTurn);
  shaftTurns = motorTurns / gearRatio;
  
  //Serial.println(encoder_pos);

  Serial.print("Encoder: ");
  Serial.print(encoder_pos);
  Serial.print("\tMotor: ");
  Serial.print(motorTurns);
  Serial.print("\tShaft: ");
  Serial.println(shaftTurns);
  delay(10);
}



void encoder(){

  if(digitalRead(encoderpinB) == HIGH){
    encoder_pos++;
  }else{
    encoder_pos--;
  }
}

void MotorClockwise(int power){
  if(power > 100){
    analogWrite(en, power);
    digitalWrite(inA, LOW);
    digitalWrite(inB, HIGH);
  }else{
    digitalWrite(inA, LOW);
    digitalWrite(inB, LOW);
  }
}

void MotorCounterClockwise(int power){
  if(power > 100){
    analogWrite(en, power);
    digitalWrite(inA, HIGH);
    digitalWrite(inB, LOW);
  }else{
    digitalWrite(inA, LOW);
    digitalWrite(inB, LOW);
  }
}
