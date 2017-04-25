#ifndef WIFI_CONNECTION
#define WIFI_CONNECTION
#include "Arduino.h"
#include <WiFiUdp.h>
#include <ESP8266WiFiAP.h>
#include <ESP8266WiFi.h>
#include "ESP8266HTTPClient.h"
#include "ESP8266WebServer.h"
#include "WIFI_Storage.h"

class WIFI_CONFIG
{
public:
	char wifiname[40] = { 0 };
	char wifipass[40] = { 0 };
public:
	void WIFI_Init();
	void WIFI_Server_Init();
private:

};




#endif // !WIFI_CONNECTION

