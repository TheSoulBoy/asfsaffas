/*
 * CALCULATOR
 */

#include <Wire.h>
#include <WireData.h>
#include <DallasTemperature.h>
#include <OneWire.h>
#include<QMC5883.h>

#define ONE_WIRE_BUS 5
#define SLAVE_ADDRESS 0x08

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
QMC5883 qq;

float Temp, deg;


void setup()
{
	Serial.begin(9600);
	sensors.begin();
	Wire.begin(SLAVE_ADDRESS);
	Wire.onRequest(i2cRequest);
	qq.begin(); 
	delay(100);
}


void loop()
{
	static uint_least8_t i = 999;
	if (++i % 1000 == 0)
	{
		sensors.requestTemperatures();
		Temp = sensors.getTempCByIndex(0);
		i = 0;
	}

	qq.calculate();
	deg = qq.getHeadingDegree('z');
	Serial.println(deg);

	delay(10);
}


void i2cRequest()
{ 
	wireWriteData(deg);
	wireWriteData(Temp);
}
