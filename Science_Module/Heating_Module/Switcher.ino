#define Relay1 10 //power enable
#define Relay2 12 //coil spliter
#define Cdelay  1000 //coil delay
#define Sdelay 500//split delay
#define uvRelay 8 //uv light
#define ltRelay 7 /cam led


void setup() {
  pinMode(Relay1,OUTPUT);
  pinMode(Relay2,OUTPUT);
  pinMode(uvRelay,OUTPUT);
  pinMode(ltRelay,OUTPUT);
  
  Serial.begin(9600);

}
char command;

void loop() {
  if (Serial.available())
  {command = Serial.read();

  if (command == 'O') //(Ninhydrin,benedict)base module pair activation 

   {
     mainOn();
   }
   if (command =='S' )     // top module pair activation 
   {
     coilSplit();
   }

   if (command == 'R') // power Return to base module 
   {
    baseReturn();
   }

   if (command == 'C')  // power cut off
   {
    powerOff();
   }
   if (command == 'U')  // UV light on
   {
    lightingUV();
   }
   if (command == 'L')  // LED on
   {
    lightingLED();
   }
  }
  }

void  mainOn(){ //base module power on
    digitalWrite(Relay2,LOW);
     delay(Sdelay);
    digitalWrite(Relay1,HIGH);
    Serial.println("main power on.base module pair with N/O contact energized");
}

void  coilSplit(){ //top module power on
     
   digitalWrite(Relay1,LOW);
   Serial.println("main power cut off");
   delay(Cdelay);
   digitalWrite(Relay2,HIGH);
   Serial.println("coil Shifted.connection transfared to top module");
   delay(Cdelay);
   digitalWrite(Relay1,HIGH);
   Serial.println("main power on.Top module energized");
   
}

void baseReturn(){ //power return to base module 
   digitalWrite(Relay1,LOW );
   Serial.println("main power cut off");
     delay(Sdelay);
   digitalWrite(Relay2,LOW);
   Serial.println("top module cut off");

   delay(Sdelay);
   digitalWrite(Relay1,HIGH);
   Serial.println("main power on");
   Serial.println( "power return to base module");
}

void powerOff(){//power cut off
  digitalWrite(Relay1,LOW);
     delay(Sdelay);
    digitalWrite(Relay2,LOW);
    Serial.println("stop"); 

}
void lightingUV(){//lighting 
  digitalWrite(uvRelay,HIGH);
  digitalWrite (ltRelay,LOW);
}

void lightingLED(){//lighting 
  digitalWrite(ltRelay,HIGH);
  digitalWrite (uvRelay,LOW);
}
