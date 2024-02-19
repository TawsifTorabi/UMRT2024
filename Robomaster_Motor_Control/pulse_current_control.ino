#include <SPI.h>
#include <mcp2515.h>
#include <TimerOne.h>

MCP2515 mcp2515(53);        // CS(SS) to pin 53 on MEGA2560

struct can_frame canMsgOut; // Transmitted CAN

int set_current[1] = {0};

void setMotorCurrent(int currentMotor0)
{
  canMsgOut.can_id = 0x200;
  canMsgOut.can_dlc = 8;
  canMsgOut.data[0] = (char)(currentMotor0 / 256);     // High 8 bit
  canMsgOut.data[1] = (char)(currentMotor0 % 256);     // Low 8 bit
  canMsgOut.data[2] = 0;
  canMsgOut.data[3] = 0;
  mcp2515.sendMessage(&canMsgOut);
}

void setup()
{
  Serial.begin(115200);
  SPI.begin();
  mcp2515.reset();
  mcp2515.setBitrate(CAN_1000KBPS, MCP_8MHZ);
  mcp2515.setNormalMode();
  Timer1.initialize(5000);
}

void loop()
{
  // Receive messages from the serial port
  if (Serial.available() > 0)
  {
    int currentMotor0 = Serial.parseInt();
    set_current[0] = constrain(currentMotor0, -16384, 16384);

    // Assuming there is only one motor
    setMotorCurrent(set_current[0]);
  }
}
