#include <SPI.h>
#include <mcp2515.h>

MCP2515 mcp2515(53);        // CS(SS) to pin 53 on MEGA2560

struct can_frame canMsgOut; // Transmitted CAN
struct can_frame canMsgIn;  // Received CAN

int set_current[1] = {0};

void setMotorCurrent(int currentMotor0)
{
  canMsgOut.can_id = 0x1FF;
  canMsgOut.can_dlc = 8;

  canMsgOut.data[0] = 0;
  canMsgOut.data[1] = 0;
  canMsgOut.data[2] = (char)(currentMotor0 / 256);     // High 8 bit
  canMsgOut.data[3] = (char)(currentMotor0 % 256);     // Low 8 bit
  canMsgOut.data[4] = 0;
  canMsgOut.data[5] = 0;
  canMsgOut.data[6] = 0;
  canMsgOut.data[7] = 0;
  mcp2515.sendMessage(&canMsgOut);
}

inline int toRealData(unsigned char DataH, unsigned char DataL)
{
  return int(word(DataH, DataL));
}

void setup()
{
  Serial.begin(115200);
  SPI.begin();
  mcp2515.reset();
  mcp2515.setBitrate(CAN_1000KBPS, MCP_8MHZ);
  mcp2515.setNormalMode();
}

bool sendMsg = true;
int currentVal = 0;

void loop()
{
  // Read potentiometer value from A0 and map it to the desired current range
  int potValue = analogRead(A0);
  int currentMotor0 = map(potValue, 0, 1023, 0, 16384);

  // Update set_current array and currentVal
  set_current[0] = currentMotor0;
  currentVal = set_current[0];

  // Assuming there is only one motor
  setMotorCurrent(currentVal);

  // Print CAN message after sending
  Serial.print("CAN ID: ");
  Serial.println(canMsgOut.can_id, HEX);
  for (int i = 0; i < canMsgOut.can_dlc; i++) {
    Serial.print(canMsgOut.data[i]);
    Serial.print(" ");
  }
  Serial.println();

  // Print other information
  Serial.print("Angle: ");
  Serial.print(toRealData(canMsgIn.data[0], canMsgIn.data[1]));
  Serial.print(" | ");
  Serial.print("RPM: ");
  Serial.print(toRealData(canMsgIn.data[2], canMsgIn.data[3]));
  Serial.print(" | ");
  Serial.print("Actual Current: ");
  Serial.print(toRealData(canMsgIn.data[4], canMsgIn.data[5]));
  Serial.print(" | ");
  Serial.print("T: ");
  Serial.print(canMsgIn.data[6]);
  Serial.print(" | ");
  Serial.print("InputCurr: ");
  Serial.print(currentVal);
  Serial.println();
}
