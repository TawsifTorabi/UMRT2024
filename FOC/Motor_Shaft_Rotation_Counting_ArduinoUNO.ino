//DFRobot 12v Encoded Motor 
//Step Counter
//Counts Encoder Steps, Motor Shafts Rotation, Geared Shaft Rotation
//Tawsif Torabi
//22 Dec 2023
//Wired for Arduino UNO

const int encoderPinA = 2;
const int encoderPinB = 4;
const int motorStepsPerTurn = 32;
const float gearRatio = 43.8;
volatile int encoderSteps = 0;
volatile float motorTurns = 0.0;
volatile float shaftTurns = 0.0;
volatile bool forwardDirection = true;

int enable = 9;
int in1 = 10;
int in2 = 11;

void setup() {
  pinMode(encoderPinA, INPUT);
  pinMode(encoderPinB, INPUT);

  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(enable, OUTPUT);
  
  attachInterrupt(digitalPinToInterrupt(encoderPinA), updateEncoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encoderPinB), checkDirection, CHANGE);

  Serial.begin(115200);
}

void loop() {
  
}

void updateEncoder() {
  if (digitalRead(encoderPinA) == digitalRead(encoderPinB)) {
    forwardDirection = true;
    encoderSteps++;
    motorTurns = float(encoderSteps) / float(motorStepsPerTurn);
    shaftTurns = motorTurns / gearRatio;
  }else{
    forwardDirection = false;
    encoderSteps--;
    motorTurns = float(encoderSteps) / float(motorStepsPerTurn);
    shaftTurns = motorTurns / gearRatio;
  }

  Serial.print("Encoder: ");
  Serial.print(encoderSteps);
  Serial.print("\tMotor: ");
  Serial.print(motorTurns);
  Serial.print("\tShaft: ");
  Serial.println(shaftTurns);
}

void checkDirection() {
  if (forwardDirection) {
    if (digitalRead(encoderPinA) != digitalRead(encoderPinB)) {
      forwardDirection = false;
    }
  } else {
    if (digitalRead(encoderPinA) == digitalRead(encoderPinB)) {
      forwardDirection = true;
    }
  }
}
