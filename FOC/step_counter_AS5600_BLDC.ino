#include <ESP32Servo.h> 
#include <Wire.h> //This is for i2C

Servo myservo;          // create servo object to control a servo
int servoPin = 12;      // GPIO pin used to connect the servo control (digital out)

bool isArmed = false;

//I2C pins:
//STM32: SDA: PB7 SCL: PB6
//Arduino: SDA: A4 SCL: A5
//ESP32: GPIO 22 (SCL) and GPIO 21 (SDA)

//Magnetic sensor things
int magnetStatus = 0; //value of the status register (MD, ML, MH)

int lowbyte; //raw angle 7:0
word highbyte; //raw angle 7:0 and 11:8
int rawAngle; //final raw angle
float degAngle; //raw angle in degrees (360/4096 * [value between 0-4095])

int quadrantNumber, previousquadrantNumber; //quadrant IDs
float numberofTurns = 0; //number of turns
float correctedAngle = 0; //tared angle - based on the startup value
float startAngle = 0; //starting angle
float totalAngle = 0; //total absolute angular displacement
float previoustotalAngle = 0; //for the display printing

int steps = 0;
int turnOffset = 25;

void setup()
{
  Serial.begin(115200);
  Wire.begin();
  Wire.setClock(800000L);

  // Allow allocation of all timers
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  myservo.setPeriodHertz(50);// Standard 50hz servo
  
  myservo.attach(servoPin, 500, 2400);
  
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

  if (userInput == 'y' || userInput == 'Y') {
    armESC(); // Call armESC() function if user input is 'y' or 'Y'
  }

  ReadRawAngle(); // Make a reading so the degAngle gets updated
  startAngle = degAngle; // Update startAngle with degAngle - for taring

  Serial.println("Welcome!"); // Print a welcome message
  Serial.println("AS5600"); // Print a welcome message
  delay(3000);
}


int val = 50;

void loop()
{
  if (numberofTurns < steps-turnOffset) {
    myservo.write(val);  // Rotate clockwise for positive turns
  } else if (numberofTurns > steps) {
    myservo.write(0);   // Rotate counterclockwise for negative turns
  } else {
    myservo.write(val);  // Or any other angle for the case when turns are equal to steps
  }
  
  ReadRawAngle(); //ask the value from the sensor
  correctAngle(); //tare the value
  checkQuadrant(); //check quadrant, check rotations, check absolute angular position
  
  Serial.println();
  

  Serial.println(totalAngle);
}

void ReadRawAngle()
{
  //7:0 - bits
  Wire.beginTransmission(0x36); //connect to the sensor
  Wire.write(0x0D); //figure 21 - register map: Raw angle (7:0)
  Wire.endTransmission(); //end transmission
  Wire.requestFrom(0x36, 1); //request from the sensor

  while (Wire.available() == 0); //wait until it becomes available
  lowbyte = Wire.read(); //Reading the data after the request

  //11:8 - 4 bits
  Wire.beginTransmission(0x36);
  Wire.write(0x0C); //figure 21 - register map: Raw angle (11:8)
  Wire.endTransmission();
  Wire.requestFrom(0x36, 1);

  while (Wire.available() == 0);
  highbyte = Wire.read();

  //4 bits have to be shifted to its proper place as we want to build a 12-bit number
  highbyte = highbyte << 8; //shifting to left
  //What is happening here is the following: The variable is being shifted by 8 bits to the left:
  //Initial value: 00000000|00001111 (word = 16 bits or 2 bytes)
  //Left shifting by eight bits: 00001111|00000000 so, the high byte is filled in

  //Finally, we combine (bitwise OR) the two numbers:
  //High: 00001111|00000000
  //Low:  00000000|00001111
  //      -----------------
  //H|L:  00001111|00001111
  rawAngle = highbyte | lowbyte; //int is 16 bits (as well as the word)

  //We need to calculate the angle:
  //12 bit -> 4096 different levels: 360° is divided into 4096 equal parts:
  //360/4096 = 0.087890625
  //Multiply the output of the encoder with 0.087890625
  degAngle = rawAngle * 0.087890625;

  //Serial.print("Deg angle: ");
  Serial.print(degAngle, 2); //absolute position of the encoder within the 0-360 circle
  Serial.print("            ");

}

void correctAngle()
{
  //recalculate angle
  correctedAngle = degAngle - startAngle; //this tares the position

  if (correctedAngle < 0) //if the calculated angle is negative, we need to "normalize" it
  {
    correctedAngle = correctedAngle + 360; //correction for negative numbers (i.e. -15 becomes +345)
  }
  else
  {
    //do nothing
  }
  //Serial.print("Corrected angle: ");
  //Serial.println(correctedAngle, 2); //print the corrected/tared angle
}

void checkQuadrant()
{
  /*
    //Quadrants:
    4  |  1
    ---|---
    3  |  2
  */

  //Quadrant 1
  if (correctedAngle >= 0 && correctedAngle <= 90)
  {
    quadrantNumber = 1;
  }

  //Quadrant 2
  if (correctedAngle > 90 && correctedAngle <= 180)
  {
    quadrantNumber = 2;
  }

  //Quadrant 3
  if (correctedAngle > 180 && correctedAngle <= 270)
  {
    quadrantNumber = 3;
  }

  //Quadrant 4
  if (correctedAngle > 270 && correctedAngle < 360)
  {
    quadrantNumber = 4;
  }
  //Serial.print("Quadrant: ");
  //Serial.println(quadrantNumber); //print our position "quadrant-wise"

  if (quadrantNumber != previousquadrantNumber) //if we changed quadrant
  {
    if (quadrantNumber == 1 && previousquadrantNumber == 4)
    {
      numberofTurns++; // 4 --> 1 transition: CW rotation
    }

    if (quadrantNumber == 4 && previousquadrantNumber == 1)
    {
      numberofTurns--; // 1 --> 4 transition: CCW rotation
    }
    //this could be done between every quadrants so one can count every 1/4th of transition

    previousquadrantNumber = quadrantNumber;  //update to the current quadrant

  }
  //Serial.print("Turns: ");
  Serial.print(numberofTurns,0); //number of turns in absolute terms (can be negative which indicates CCW turns)
  Serial.print("              "); //number of turns in absolute terms (can be negative which indicates CCW turns)

  //after we have the corrected angle and the turns, we can calculate the total absolute position
  totalAngle = (numberofTurns * 360) + correctedAngle; //number of turns (+/-) plus the actual angle within the 0-360 range
  //Serial.print("Total angle: ");
  //Serial.println(totalAngle, 2); //absolute position of the motor expressed in degree angles, 2 digits
}



void armESC() {
  if(isArmed == false){
    Serial.println("Arming the ESC");
    int i, j;
    // Send a low throttle signal to arm the ESC
    for(i=0; i<=90; i++){
       myservo.write(i);
       delay(200);
    }
    for(j=90; j<=0; j--){
       myservo.write(j);
       delay(100);
    }
    myservo.write(0);
    delay(1000); // Wait for 2 seconds to allow the ESC to arm
    Serial.println("ESC Armed");
  }
  isArmed = true;
}
