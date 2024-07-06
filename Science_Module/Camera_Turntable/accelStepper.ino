#include <AccelStepper.h>
#include <PS4Controller.h> // Include the library for PS4 controller input

// Stepper Driver Pins
const int dirPin = 5;
const int stepPin = 18; // 18 gpio
const int enPin = 19;


AccelStepper stepper(AccelStepper::DRIVER, stepPin, dirPin);

void setup()
{
  // Change these to suit your stepper if you want
  stepper.setMaxSpeed(10000);
  stepper.setAcceleration(100);
  stepper.moveTo(0);


  // Initialize the PS4 controller
  PS4.begin();
  Serial.begin(115200);
}


// Maximum steps
int maxStepStepper = 1195; //Mapped for 360 deg. on camera turntable

const int StepperStepToGo = 50; 
void moveAndDriveStepper()
{
  while (PS4.Left() || PS4.Right())
  {
    // If PS4 left button pressed and within the range
    if (PS4.Left() && stepper.currentPosition() < maxStepStepper) {
      // Move the stepper motor forward
      int targetPosition = stepper.currentPosition() + StepperStepToGo;
      targetPosition = min(targetPosition, maxStepStepper);
      stepper.moveTo(targetPosition);
      stepper.run();
      Serial.print("Dir 1 - ");
      Serial.print(stepper.currentPosition());
      Serial.println();
    }
    // If PS4 right button pressed and within the range
    else if (PS4.Right() && stepper.currentPosition() > -maxStepStepper) {
      // Move the stepper motor backward
      int targetPosition = stepper.currentPosition() - StepperStepToGo;
      targetPosition = max(targetPosition, -maxStepStepper);
      stepper.moveTo(targetPosition);
      stepper.run();
      Serial.print("Dir 2 - ");
      Serial.print(stepper.currentPosition());
      Serial.println();
    }
    // If maximum step is reached, stop the stepper motor
    else {
      stepper.stop();
      break;
    }
  }
}




void loop()
{
  // Move and drive the stepper motor
  moveAndDriveStepper();
}
