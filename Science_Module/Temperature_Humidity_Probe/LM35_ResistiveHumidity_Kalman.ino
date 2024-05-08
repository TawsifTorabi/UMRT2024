/*
Using Simple Kalman Filter to Stabilize the Values
And to get the Max Humidity Value, submerge the sensor into water to get the maximum sensor analog value.
Take a reading of the analog value, and then set the max sensor value.

In the open air, the value should be 0. If the value fluctuates in the open air, there are some water
or humidity in the sensor probes thats are causing electrolysis to create a voltage across the bridge. 
*/


int moistPin = A0;
int lm35Pin = A2;
int moistVal = 0;

double kalmanEstimate = 0;
double kalmanGain = 0.1; // Adjust the Kalman gain as needed

int maxSensorValue = 1018;

void setup()
{
  Serial.begin(9600);
}

double kalmanFilter(double measurement)
{
  static double prevEstimate = 0;
  // Prediction update
  kalmanEstimate = prevEstimate;
  // Measurement update (correction)
  kalmanEstimate = kalmanEstimate + kalmanGain * (measurement - kalmanEstimate);
  prevEstimate = kalmanEstimate;
  
  // Apply saturation limit
  if (kalmanEstimate < 0) {
    kalmanEstimate = 0;
  } else if (kalmanEstimate > maxSensorValue) { // Assuming 730 is the maximum value
    kalmanEstimate = maxSensorValue;
  }

  return kalmanEstimate;
}

void loop()
{
  // Get the voltage reading from the LM35
  int reading = analogRead(lm35Pin);
  // Convert that reading into voltage
  float voltage = reading * (5.0 / 1024.0);
  // Convert the voltage into the temperature in Celsius
  float temperatureC = voltage * 100 +2;

  // Print the temperature in Celsius
  Serial.print("Temperature: ");
  Serial.print(temperatureC);
  Serial.print("\xC2\xB0"); // shows degree symbol
  Serial.print("C  |  ");

  // Print the temperature in Fahrenheit
  float temperatureF = (temperatureC * 9.0 / 5.0) + 32.0;
  Serial.print(temperatureF);
  Serial.print("\xC2\xB0"); // shows degree symbol
  Serial.print("F");
  Serial.print(" | ");

  delay(500);


  moistVal = analogRead(moistPin);
  //Serial.println(moistVal);

  double smoothedMoistVal = kalmanFilter(moistVal);

  // Calculate percentage
  double percent = (smoothedMoistVal / maxSensorValue) * 100; // Assuming 730 is the maximum value

  // Ensure percentage does not exceed 100%
  if (percent > 100) {
    percent = 100;
  }

  Serial.print(percent);
  Serial.println("% Moisture ");
  delay(500);
}
