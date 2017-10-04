//#define CAYENNE_DEBUG
#define CAYENNE_PRINT Serial
#include <CayenneMQTTESP8266.h>
//#include <CayenneTemperature.h>
#include "DHT.h"
#include "iot_rahasia.h"

#define DHTPIN 2
#define DHTTYPE DHT22
#define VIRTUAL_HUM V1
#define VIRTUAL_TEMP V2

DHT dht(DHTPIN, DHTTYPE);

// WiFi network info.
char ssid[] = IOT_SSID;
char wifiPassword[] = IOT_WIFI_PWD;

// Cayenne authentication info. This should be obtained from the Cayenne Dashboard.
char username[] = CAYENNE_USERNAME;
char password[] = CAYENNE_PWD;
char clientID[] = CAYENNE_ID;

unsigned long lastMillis = 0;

void setup() {
	Serial.begin(9600);
  dht.begin();
	Cayenne.begin(username, password, clientID, ssid, wifiPassword);
}

//CAYENNE_OUT(V0)
//{
//  float t = dht.readTemperature();
//  Cayenne.virtualWrite(V0, t); //virtual pin
//}

//CAYENNE_OUT(V1)
//{
//  float h = dht.readHumidity();
//  Cayenne.virtualWrite(V1, h); //virtual pin
//}

void loop() {
	Cayenne.loop();
  float t = dht.readTemperature();
  float h = dht.readHumidity();
  
	//Publish data every 10 seconds (10000 milliseconds). Change this value to publish at a different interval.
	if (millis() - lastMillis > 10000) {
		lastMillis = millis();
		//Write data to Cayenne here. This example just sends the current uptime in milliseconds.
		//Cayenne.virtualWrite(0, lastMillis);
		//Some examples of other functions you can use to send data.
		//Cayenne.celsiusWrite(1, 22.00);
		//Cayenne.luxWrite(2, 700);
		//Cayenne.virtualWrite(3, 50, TYPE_PROXIMITY, UNIT_CENTIMETER);
   Cayenne.virtualWrite(V1, h);
   Cayenne.virtualWrite(V2, t);
	}
}

//Default function for processing actuator commands from the Cayenne Dashboard.
//You can also use functions for specific channels, e.g CAYENNE_IN(1) for channel 1 commands.
CAYENNE_IN_DEFAULT()
{
	CAYENNE_LOG("CAYENNE_IN_DEFAULT(%u) - %s, %s", request.channel, getValue.getId(), getValue.asString());
	//Process message here. If there is an error set an error message using getValue.setError(), e.g getValue.setError("Error message");
}
