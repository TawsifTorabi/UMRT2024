#include <SPI.h>
#include <mcp2515.h>
#include <Kalman.h>

MCP2515 mcp2515(53); // CS(SS) to pin 53 on MEGA2560

struct can_frame canMsgIn; // Received CAN message

Kalman myFilter(0.05, 16, 1023, 0); // Kalman filter

int angle_last[2] = {0};
int position[2] = {0};

void initCAN()
{
  SPI.begin();
  mcp2515.reset();
  mcp2515.setBitrate(CAN_1000KBPS, MCP_8MHZ);
  mcp2515.setNormalMode();
}

void updatePosition(int motorID, int angle)
{
  // Process position based on angle changes
  int delta_pos = angle - angle_last[motorID];
  // Update position accordingly
  if (abs(delta_pos) < 8000) // Normal position update
  {
    position[motorID] += delta_pos;
  }
  else if (delta_pos < -8000) // Full circle rotation counter-clockwise
  {
    position[motorID] += (delta_pos + 16384);
  }
  else if (delta_pos > 8000) // Full circle rotation clockwise
  {
    position[motorID] += (delta_pos - 16384);
  }
  angle_last[motorID] = angle;
}

int convertToReadableRPM(int rawRPM)
{
  // Conversion logic for raw RPM to readable format
  // Adjust this according to your specific requirements
  return rawRPM * 60 / 4096; // Assuming rawRPM is in a certain range, adjust accordingly
}

void setup()
{
  Serial.begin(115200);
  initCAN();
}

void loop()
{
  if (CANmsgComing())
  {
    // Filter and process CAN message
    int angle = toRealData(canMsgIn.data[0], canMsgIn.data[1]);
    int filtered_RPM = (int)myFilter.getFilteredValue(toRealData(canMsgIn.data[2], canMsgIn.data[3]));

    // Update position
    updatePosition(canMsgIn.can_id - 0x201, angle);

    // Convert RPM to readable format
    int readable_RPM = convertToReadableRPM(filtered_RPM);

    // Print processed CAN message data
    Serial.print("Position: ");
    Serial.print(position[0]); // Assuming only one motor, adjust as necessary
    Serial.print("\tFiltered RPM: ");
    Serial.print(filtered_RPM);
    Serial.print("\tReadable RPM: ");
    Serial.println(readable_RPM);
  }
}

inline bool CANmsgComing()
{
  return mcp2515.readMessage(&canMsgIn) == MCP2515::ERROR_OK;
}

inline int toRealData(unsigned char DataH, unsigned char DataL)
{
  return int(word(DataH, DataL));
}
