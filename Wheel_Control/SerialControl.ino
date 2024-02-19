#include <SoftwareSerial.h>
#include <SabertoothSimplified.h>


SoftwareSerial UMRTSerial(12,14);//RX for pin 12,TX for pin 14.RX is S2 and tx is S1
SabertoothSimplified ST(UMRTSerial);

void setup() 
{
  Serial.begin(9600);
  UMRTSerial.begin(9600); 
  
}
char command;

void loop() {

  if (Serial.available()){
  command = Serial.read();

  if (command == 'F')
  {
    ST.motor(1, 127);
    ST.motor(2 ,-127);
    Serial.print("F");
    Serial.println("both motor will turn forward");
    delay(3000);
    
  }

  else if (command == 'B')
  {
    ST.motor(1,-127);
    ST.motor(2,127);
    Serial.print("B");
    Serial.println("both motor will turn reverse");
    delay(3000);
    
    
  }
else if (command == 'R')
{
  ST.motor(2,-127);
  ST.motor(1,-127);
  Serial.print('R');
  Serial.println("bot will turn right");
  delay(3000);
    
}
else if (command == 'L')
{
  ST.motor(2,127);
  ST.motor(1,127);
  Serial.print('L');
  Serial.println("bot will turn reverse");
  delay(3000);
    
}

}

else {
  ST.motor(1,0);
  ST.motor(2,0);
  Serial.print("stop");
  Serial.println("bot will stand still");
}
  
 
}
