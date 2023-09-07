#include <ardubsonStreamParser.h>
#include <ardubsonDocument.h>
#include <ardubsonObject.h>
#include <ardubson.h>
#include <ardubsonConfig.h>
#include <ardubsonTypes.h>
#include <IEEE754tools.h>
#include <ardubsonObjBuilder.h>
#include <ardubsonElement.h>

#define BSON_RPM "rpm"
#define BSON_GEARSHIFT "gear"
#define BSON_ENGINETEMPERATURE "eng_t"
#define BSON_OILPRESSURE "oil_p"
#define BSON_OILTEMPERATURE "oil_t"
#define BSON_BATTERYVOLTAGE "bat_v"
#define BSON_VEHICLESPEED "vel"
#define BSON_DATALOGGERSTATUS "dtl_s"
#define BSON_AFR "af_r"
#define BSON_TCSLIP "tc_s"
#define BSON_TCLAUNCH "tc_l"

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
	RPM+=500;
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
	//8 bit, Odd parity and 1 bit for stop
	Serial.begin(115200,SERIAL_8O1);
	//set odd parity for arduino 
	
	delay(2000);
	bob.append(BSON_RPM, (int32_t)RPM);
	bob.append(BSON_BATTERYVOLTAGE, (int32_t)g_OilPressure.encodedValue); //float 
	bob.append(BSON_OILTEMPERATURE,  (int32_t)g_OilPressure.encodedValue); //float
	bob.append(BSON_OILPRESSURE,  (int32_t)g_OilPressure.encodedValue); //float
	bob.append(BSON_ENGINETEMPERATURE, (int32_t) RPM);
	bob.append(BSON_VEHICLESPEED, (int32_t) RPM);
	bob.append(BSON_AFR, (int32_t)g_OilPressure.encodedValue);
	bob.append(BSON_GEARSHIFT, (int32_t) GEAR);
	bob.append(BSON_DATALOGGERSTATUS, (int32_t) RPM);
	bob.append(BSON_TCSLIP, (int32_t) RPM);
	bob.append(BSON_TCLAUNCH, (int32_t) RPM);
	delay(100);
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
		bo.updateField(BSON_RPM ,RPM);
		bo.updateField(BSON_BATTERYVOLTAGE ,(int32_t)g_OilPressure.encodedValue); 
		bo.updateField(BSON_OILTEMPERATURE ,(int32_t)g_OilPressure.encodedValue);
		bo.updateField(BSON_OILPRESSURE ,(int32_t)g_OilPressure.encodedValue);
	  	bo.updateField(BSON_ENGINETEMPERATURE ,(int32_t)RPM);
		bo.updateField(BSON_VEHICLESPEED ,(int32_t)RPM);
		bo.updateField(BSON_AFR ,(int32_t)g_OilPressure.encodedValue);
		bo.updateField(BSON_GEARSHIFT ,(int32_t)GEAR);
		bo.updateField(BSON_DATALOGGERSTATUS ,(int32_t)RPM);
		bo.updateField(BSON_TCSLIP,(int32_t)RPM); 
		bo.updateField(BSON_TCLAUNCH,(int32_t)RPM);
	  
		Serial.print(bsonW);
		Serial.write(bo.rawData(), a);
		milliss=millist;
	}	
		
	//Serial.print(millis()-millist);
		
}
