//32 steps in each turn
const int encoderPinA = 2;
const int encoderPinB = 4;

volatile int steps = 0;
volatile bool forwardDirection = true;

void setup() {
  pinMode(encoderPinA, INPUT);
  pinMode(encoderPinB, INPUT);
  
  attachInterrupt(digitalPinToInterrupt(encoderPinA), updateEncoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encoderPinB), checkDirection, CHANGE);

  Serial.begin(115200);
}

void loop() {
  // Do other tasks here if needed
}

void updateEncoder() {
  if (digitalRead(encoderPinA) == digitalRead(encoderPinB)) {
    forwardDirection = true;
    steps++;
  } else {
    forwardDirection = false;
    steps--;
  }
  
  Serial.println(steps);
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
