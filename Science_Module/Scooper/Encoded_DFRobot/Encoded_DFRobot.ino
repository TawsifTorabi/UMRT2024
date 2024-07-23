//Encoder Pins
const int encoderPinA = 26;
const int encoderPinB = 25;

//Declared Constants
const int motorStepsPerTurn = 32;
const float gearRatio = 43.8;

//Variables
volatile int encoderSteps = 0;
volatile float motorTurns = 0.0;
volatile float shaftTurns = 0.0;

//Motor Driver Pins
int enable = 12;
int in1 = 14;
int in2 = 27;

int steps = 0;


//Flags
volatile bool forwardDirection = true;
bool moveRequested = false;
float targetShaftTurns = 0.0;

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
  Serial.print("Enter Steps: ");
  while (Serial.available() == 0) {
    // Wait until a value is received through Serial
  }

  steps = Serial.parseInt();  // Read an integer from Serial

  Serial.println();

  // Prompt user to arm the ESC
  Serial.print("Do you want to arm the ESC? (y/n): ");

  // Wait for valid input
  char userInput = 0;
  while (userInput != 'y' && userInput != 'Y' && userInput != 'n' && userInput != 'N') {
    if (Serial.available() > 0) {
      userInput = Serial.read(); // Read a single character from Serial
    }
  }

  

  if (moveRequested) {
    moveMotorToPosition(targetShaftTurns);
    moveRequested = false;
  }
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

  // Serial.print("Encoder: ");
  // Serial.print(encoderSteps);
  // Serial.print("\tMotor: ");
  // Serial.print(motorTurns);
  // Serial.print("\tShaft: ");
  // Serial.println(shaftTurns);
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


void handleSerialCommand(char command) {
  switch (command) {
    case 'f':
      Serial.println("Enter target shaft turns:");
      while (!Serial.available()) {
        // Wait until data is available
      }
      targetShaftTurns = Serial.parseFloat();
      Serial.print("Input: ");
      Serial.println(targetShaftTurns);
      moveRequested = true;
      break;
    case 's':
      // Stop the motor
      analogWrite(enable, 0); // Stop PWM
      digitalWrite(in1, LOW);
      digitalWrite(in2, LOW);
      break;
    case 'r':
      // Reverse the motor
      // Implement reversing functionality if needed
      break;
    default:
      break;
  }
}

void moveMotorToPosition(float targetPosition) {
  float currentShaftTurns = shaftTurns;

  while (currentShaftTurns < targetPosition) {
    // Move motor forward
    analogWrite(enable, 80); // Adjust PWM as needed
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    // You might need to add delay here depending on the motor speed and response time
    currentShaftTurns = shaftTurns;



  }

  // Stop the motor after reaching the target position
  if(currentShaftTurns > targetPosition)
  {
  analogWrite(enable, 0); // Stop PWM
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
}
}
