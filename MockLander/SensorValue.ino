void setup() {
  Serial.begin(9600); // Initialize serial communication at 9600 baud rate
}

void loop() {
  int sensorValueA0 = analogRead(A0); // Read analog value from pin A0
  int sensorValueA1 = analogRead(A1); // Read analog value from pin A1
  
  Serial.print("A0: ");
  Serial.print(sensorValueA0); // Print the analog value from pin A0
  Serial.print(" - ");
  Serial.print("A1: ");
  Serial.print(sensorValueA1); // Print the analog value from pin A1
  Serial.println();
  delay(100); // Delay for 1 second
}
