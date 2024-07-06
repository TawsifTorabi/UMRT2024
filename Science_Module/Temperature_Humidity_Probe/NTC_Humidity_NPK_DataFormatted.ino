#include <SoftwareSerial.h>
#include <Wire.h>



// RE and DE Pins set the RS485 module
// to Receiver or Transmitter mode
#define RE 8
#define DE 7

float ph = 0;

// Modbus RTU requests for reading NPK values
const byte nitro[] = {0x01,0x03, 0x00, 0x1e, 0x00, 0x01, 0xe4, 0x0c};
const byte phos[] = {0x01,0x03, 0x00, 0x1f, 0x00, 0x01, 0xb5, 0xcc};
const byte pota[] = {0x01,0x03, 0x00, 0x20, 0x00, 0x01, 0x85, 0xc0};
const byte ph_cmd[] = {0x01,0x03, 0x00, 0x06, 0x00, 0x01, 0x64, 0x0b};//0x0B64
const byte temp_cmd[] = {0x01,0x03, 0x00, 0x13, 0x00, 0x01, 0x75, 0xcf};


// A variable used to store NPK values
byte values[11];


// Sets up a new SoftwareSerial object
// Digital pins 10 and 11 should be used with a Mega or Mega 2560
SoftwareSerial mod(2, 3);
//SoftwareSerial mod(10, 11);

int moistPin = A0;
int moistVal = 0;
const int thermistorPin = A3;
int maxSensorValue = 1018;


unsigned long delayTime;

void setup() {
  Serial.begin(9600);
  
  // Set the baud rate for the SerialSoftware object
  mod.begin(4800);

  // Define pin modes for RE and DE
  pinMode(RE, OUTPUT);
  pinMode(DE, OUTPUT);

  bool status;
  delayTime = 100;

  delay(500);
}


// Thermistor constants
const float A = 0.001129148;
const float B = 0.000234125;
const float C = 0.0000000876741;

void loop() { 
  
  // Read the thermistor value
  int thermistorValue = analogRead(thermistorPin);
  // Calculate the resistance of the thermistor
  float resistance = (1023.0 / thermistorValue) - 1.0;
  resistance = 21000.0 / resistance;
  //29000

  // Calculate the temperature using the Steinhart-Hart equation
  float temperature = 1.0 / (A + B * log(resistance) + C * pow(log(resistance), 3));
  temperature -= 273.15; // Convert Kelvin to Celsius
  //temperature = kalmanFilter(temperature);

  
  moistVal = analogRead(moistPin);
  //Serial.println(moistVal);
  double smoothedMoistVal = kalmanFilter(moistVal);

  // Calculate percentage
  double percent = (smoothedMoistVal / maxSensorValue) * 100; // Assuming 730 is the maximum value

  // Ensure percentage does not exceed 100%
  if (percent > 100) {
    percent = 100;
  }
  
  //NPK Values
  byte nitrogenValue = nitrogen();
  delay(delayTime);
  byte phosphorousValue = phosphorous();
  delay(delayTime);
  byte potassiumValue = potassium();
  delay(delayTime);
  byte pHValue = phValue();
  delay(delayTime);
  // byte temperatureVal = temperatureValue();
  // delay(delayTime);

  ph = pHValue;
  if (ph > 13) {
    ph -= 13;
  }

  // Print the temperature
  Serial.println("Temperature: " + String(temperature));
  //Serial.print(kalmanFilter(temperature));
  delay(delayTime);


  Serial.println("Humidity: " + String(percent));
  //Serial.print(kalmanFilter(temperature));
  delay(delayTime);

  Serial.println("Nitrogen: " + String(nitrogenValue));
  Serial.println("Phosphorous: " + String(phosphorousValue));
  Serial.println("Potassium: " + String(potassiumValue));
  Serial.println("pH: " + String(ph));
  // Serial.print("Temperature: " + String(temperatureVal));
  
  //Serial.println();
  
}



 
byte nitrogen(){
  digitalWrite(DE,HIGH);
  digitalWrite(RE,HIGH);
  delay(10);
  if(mod.write(nitro,sizeof(nitro))==8){
    digitalWrite(DE,LOW);
    digitalWrite(RE,LOW);
    for(byte i=0;i<7;i++){
    //Serial.print(mod.read(),HEX);
    values[i] = mod.read();
    //Serial.print(values[i],HEX);
    }
    //Serial.println();
  }
  return values[4];
}
 
byte phosphorous(){
  digitalWrite(DE,HIGH);
  digitalWrite(RE,HIGH);
  delay(10);
  if(mod.write(phos,sizeof(phos))==8){
    digitalWrite(DE,LOW);
    digitalWrite(RE,LOW);
    for(byte i=0;i<7;i++){
    //Serial.print(mod.read(),HEX);
    values[i] = mod.read();
    //Serial.print(values[i],HEX);
    }
    //Serial.println();
  }
  return values[4];
}
 
byte potassium(){
  digitalWrite(DE,HIGH);
  digitalWrite(RE,HIGH);
  delay(10);
  if(mod.write(pota,sizeof(pota))==8){
    digitalWrite(DE,LOW);
    digitalWrite(RE,LOW);
    for(byte i=0;i<7;i++){
    //Serial.print(mod.read(),HEX);
    values[i] = mod.read();
    //Serial.print(values[i],HEX);
    }
    //Serial.println();
  }
  return values[4];
}

byte phValue(){
  digitalWrite(DE, HIGH);
  digitalWrite(RE, HIGH);
  delay(10);
  if(mod.write(ph_cmd, sizeof(ph_cmd)) == 8){
    digitalWrite(DE, LOW);
    digitalWrite(RE, LOW);
    for(byte i = 0; i < 7; i++){
      values[i] = mod.read();
    }
  }
    byte pH = values[4]; // Get the pH value
  
  // Subtract 13 only if the pH value is greater than 13
  // if (pH > 13) {
  //   pH -= 13;
  // }
  
  return pH;

}

// byte temperatureValue(){
//   digitalWrite(DE, HIGH);
//   digitalWrite(RE, HIGH);
//   delay(10);
//   if(mod.write(temp_cmd, sizeof(temp_cmd)) == 8){
//     digitalWrite(DE, LOW);
//     digitalWrite(RE, LOW);
//     for(byte i = 0; i < 7; i++){
//       values[i] = mod.read();
//     }
//   }
//   return values[4];
// }

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
