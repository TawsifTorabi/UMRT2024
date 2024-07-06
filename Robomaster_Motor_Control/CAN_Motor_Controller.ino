#include <SPI.h>
#include <mcp2515.h>

struct can_frame canMsg1;
MCP2515 mcp2515(10);

const int potPin = A0;  // Analog pin connected to the potentiometer

void setup() {
  canMsg1.can_id  = 0x206;  // Change the CAN ID to 0x201
  canMsg1.can_dlc = 8;      // Data Length Code (DLC) is 8 bytes
  canMsg1.data[0] = 0x00;   // Initialize with 0, will be updated with potentiometer value
  canMsg1.data[1] = 0x00;   // Initialize with 0, will be updated with potentiometer value
  canMsg1.data[2] = 0x00;   // Placeholder for additional data if needed
  canMsg1.data[3] = 0x00;   // Placeholder for additional data if needed
  canMsg1.data[4] = 0x00;   // Placeholder for additional data if needed
  canMsg1.data[5] = 0x00;   // Placeholder for additional data if needed
  canMsg1.data[6] = 0x00;   // Placeholder for additional data if needed
  canMsg1.data[7] = 0x00;   // Placeholder for additional data if needed

  while (!Serial);
  Serial.begin(115200);

  mcp2515.reset();
  mcp2515.setBitrate(CAN_1000KBPS);
  mcp2515.setNormalMode();

  Serial.println("Example: Write to CAN");
}

void loop() {
  // Read the potentiometer value
  int potValue = analogRead(potPin);

  // Map the potentiometer value to the range of -16384 to 16384
  int current = map(potValue, 0, 1023, -16384, 16384);

  // Update the CAN message data bytes
  canMsg1.data[0] = (current >> 8) & 0xFF;  // Higher order byte
  canMsg1.data[1] = current & 0xFF;         // Lower order byte

  // Print potentiometer value, mapped current value, and the CAN message to Serial Monitor
  Serial.print("Potentiometer Value: ");
  Serial.print(potValue);
  Serial.print("\tMapped Current: ");
  Serial.print(current);
  Serial.print("\tMessage Sent: ");
  for (int i = 0; i < canMsg1.can_dlc; i++) {
    Serial.print(canMsg1.data[i], HEX);
    Serial.print(" ");
  }
  Serial.println();

  // Send the updated CAN message
  mcp2515.sendMessage(&canMsg1);

  delay(100);  // Add a delay to avoid flooding the CAN bus
}
