/*
 * Complete Project Details https://randomnerdtutorials.com
*/

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10

#define SEALEVELPRESSURE_HPA (1013.25)

const int thermistorPin = A3;

Adafruit_BME280 bme; // I2C
//Adafruit_BME280 bme(BME_CS); // hardware SPI
//Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK); // software SPI

unsigned long delayTime;

void setup() {
  Serial.begin(9600);
  bool status;
  // default settings
  // (you can also pass in a Wire library object like &Wire2)
  status = bme.begin(0x76);  
  if (!status) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }
  
  Serial.println("-- Default Test --");
  delayTime = 300;

  Serial.println();
}


// Thermistor constants
const float A = 0.001129148;
const float B = 0.000234125;
const float C = 0.0000000876741;

double kalmanEstimate = 0;
double kalmanGain = 0.1; // Adjust the Kalman gain as needed

double kalmanFilter(double measurement){
  static double prevEstimate = 0;
  // Prediction update
  kalmanEstimate = prevEstimate;
  // Measurement update (correction)
  kalmanEstimate = kalmanEstimate + kalmanGain * (measurement - kalmanEstimate);
  prevEstimate = kalmanEstimate;

  return kalmanEstimate;
}


void loop() { 
  printValues();
  
  // Read the thermistor value
  int thermistorValue = analogRead(thermistorPin);
  // Calculate the resistance of the thermistor
  float resistance = (1023.0 / thermistorValue) - 1.0;
  resistance = 29000.0 / resistance;

  // Calculate the temperature using the Steinhart-Hart equation
  float temperature = 1.0 / (A + B * log(resistance) + C * pow(log(resistance), 3));
  temperature -= 273.15; // Convert Kelvin to Celsius

  // Print the temperature
  Serial.print("NTC Temperature = ");
  Serial.print(temperature);
  //Serial.print(kalmanFilter(temperature));
  Serial.println("°C");


  delay(delayTime);
}


void printValues() {
  Serial.print("BME Temperature = ");
  Serial.print(bme.readTemperature());
  Serial.print(" °C | ");
}
