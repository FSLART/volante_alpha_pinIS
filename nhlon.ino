#include <ardubsonStreamParser.h>
#include <ardubsonDocument.h>
#include <ardubsonObject.h>
#include <ardubson.h>
#include <ardubsonConfig.h>
#include <ardubsonTypes.h>
#include <IEEE754tools.h>
#include <ardubsonObjBuilder.h>
#include <ardubsonElement.h>

int RPM=0;
int ET = 0, POT;
float OP = 0;
int32_t OPint = 0;
unsigned long millist=0;
unsigned long milliss=0;
int GEAR=0;
typedef union  {
  int32_t encodedValue;
  float decodedValue;
}EncodingUnion;

EncodingUnion g_OilPressure; 

String bsonW="\xFF\xFF\xFF\xFF";
int period=90;
void setup() {
    // Setup serial port
    Serial.begin(115200);

    delay(3000);
    

 

    // Create a BSON Builder

}

void loop(){
  
    simulateCAN();
    millist=millis();
    BSONObjBuilder bob;
    
    // Append an element to BSON Builder
    bob.append("rpm", (int32_t)RPM);
    //bob.append("bat_voltage", (int32_t)g_OilPressure.encodedValue); //float 
    //bob.append("oil_temp",  (int32_t)g_OilPressure.encodedValue); //float
    //bob.append("oil_press",  (int32_t)g_OilPressure.encodedValue); //float
    //bob.append("eng_temp", (int32_t) RPM);
    //bob.append("velocity", (int32_t) RPM);
    
    BSONObject bo = bob.obj();
    int a =  bo.len();
    if(millist-milliss>=period){
      Serial.print(bsonW);
 
      Serial.write(bo.rawData(), a);
      milliss=millist;
    }    
    
    //Serial.print(millis()-millist);
    
}
void simulateCAN(){
  RPM+=10;
  ET ++;
  OP+=0.1;
  if(GEAR>=6){
    GEAR=0;
  }
  if(RPM>5000){
    RPM=0;
    GEAR+=1;
  }
  if(ET>120){
    ET=0;
  }
  if(OP>10){
    OP=0;
  }
  OPint = OP * 10;
}
void printHex(char *data, int len)
{
    for (int i = 0; i < len; i++, data++)
    {
        Serial.print("0x");
        if ((unsigned char)*data <= 0xF)
            Serial.print("0");
        Serial.print((unsigned char)*data, HEX);
        Serial.print(" ");
        if ((i + 1) % 0x8 == 0)
            Serial.println();
    }
    Serial.println();
}
