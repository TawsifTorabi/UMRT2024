#include <SoftwareSerial.h>
#include <SabertoothSimplified.h>
#include <BluetoothSerial.h>
BluetoothSerial UMRTSBT;
char incoming;//vallue will be stored as integer in this variable

SoftwareSerial UMRTSerial(12,14);//RX for pin 12,TX for pin 14.RX is S2 and tx is S1
SabertoothSimplified ST(UMRTSerial);

void setup() 
{
  Serial.begin(9600);
  UMRTSerial.begin(9600);
 
  UMRTSBT.begin("UMRTSBT");
  
}
void Forward(){
    ST.motor(1, 127);
    ST.motor(2 ,-127);
    Serial.print("F ");
    //Serial.println("both motor will turn forward");
}
void Backward(){
  ST.motor(1,-127);
    ST.motor(2,127);
    Serial.print("B ");
    //Serial.println("both motor will turn reverse");
}
void TurnRight(){
 ST.motor(2,-127);
  ST.motor(1,-127);
  Serial.print("R");
  //Serial.println("bot will turn right");

    
}
void TurnLeft(){
  ST.motor(2,127);
  ST.motor(1,127);
  Serial.print("L");
  //Serial.println("bot will turn left");
}

void Stop(){
  ST.motor(1,0);
  ST.motor(2,0);
  Serial.print("stop");
 // Serial.println("bot will stand still");
 
}

void loop() {
if (UMRTSBT.available())
{
  incoming = UMRTSBT.read();
  Serial.print("recived");
  Serial.println("incoming");

  if(incoming== 'S')
  {
    Stop();
  }
if(incoming=='F')
{Forward();
  
}
if(incoming=='B')
{Backward();
 
}
if(incoming=='R')
{TurnRight();
}
if(incoming=='L')
{TurnLeft();
}

}
}
