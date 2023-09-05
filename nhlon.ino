#include <ardubsonStreamParser.h>
#include <ardubsonDocument.h>
#include <ardubsonObject.h>
#include <ardubson.h>
#include <ardubsonConfig.h>
#include <ardubsonTypes.h>
#include <IEEE754tools.h>
#include <ardubsonObjBuilder.h>
#include <ardubsonElement.h>

int32_t RPM=0;
int32_t ET = 0, POT;
float OP = 0;
int32_t OPint = 0;
unsigned long millist=0;
unsigned long milliss=0;
int32_t GEAR=0;

typedef union  {
  int32_t encodedValue;
  float decodedValue;
}EncodingUnion;

EncodingUnion g_OilPressure; 

String bsonW="\xFF\xFF\xFF\xFF";
int period=500;
BSONObjBuilder bob;
void simulateCAN(){
	g_OilPressure.encodedValue +=10;
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

void setup() {
	// Setup serial port
	Serial.begin(115200);

	delay(3000);
	bob.append("rpm", (int32_t)RPM);
	bob.append("bat_voltage", (int32_t)g_OilPressure.encodedValue); //float 
	bob.append("oil_temp",  (int32_t)g_OilPressure.encodedValue); //float
	bob.append("oil_press",  (int32_t)g_OilPressure.encodedValue); //float
	/*bob.append("eng_temp", (int32_t) RPM);
	bob.append("velocity", (int32_t) RPM);
	bob.append("af_ratio", (int32_t)g_OilPressure.encodedValue);
	bob.append("gear", (int32_t) GEAR);
	bob.append("dl_status", (int32_t) RPM);
	bob.append("tc_slip", (int32_t) RPM);
	bob.append("tc_launch", (int32_t) RPM);*/
	delay(3000);
}
void loop(){
	BSONObject bo =bob.obj();
	int a =  bo.len();
   
	millist=millis();
	/* BSONObject bo = bob.obj();
		int a =  bo.len();
	*/ 
	if(millist-milliss>=period){
		simulateCAN();
		bo.updateField("rpm",RPM);
		bo.updateField("bat_voltage",(int32_t)g_OilPressure.encodedValue); 
		bo.updateField("oil_temp",(int32_t)g_OilPressure.encodedValue);
		bo.updateField("oil_press",(int32_t)g_OilPressure.encodedValue);
	  	/*bo.updateField("eng_temp",(int32_t)RPM);
		bo.updateField("velocity",(int32_t)RPM);
		bo.updateField("af_ratio",(int32_t)g_OilPressure.encodedValue);
		bo.updateField("gear",(int32_t)GEAR);
		bo.updateField("dl_status",(int32_t)RPM);
		bo.updateField("tc_slip",(int32_t)RPM); 
		bo.updateField("tc_launch",(int32_t)RPM);*/
	  
		Serial.print(bsonW);
		Serial.write(bo.rawData(), a);
		milliss=millist;
	}	
		
	//Serial.print(millis()-millist);
		
}
