/*
 Name:		Sketch1.ino
 Created:	2017/4/25 15:46:31
 Author:	yewei
*/

#include "stdio.h"
#include "stdlib.h"
#include "scheduler.h"
#include "WIFI_Connection.h"
#include "WIFI_Storage.h"
#include "WIFI_OTA.h"
#include "WIFI_TCP.h"
#include "data_transfer.h"
#include "net_time.h"
extern WIFI_CONFIG MY_WIFI;
extern ESP8266WebServer server(80);
extern int led_state = 0;

void Res_Init() {
	pinMode(BUILTIN_LED, OUTPUT);
}

void setup() {
	Serial.begin(115200);
	delay(1000);
	Serial.println("");
	Serial.println("welcome to use WIFI");
	Res_Init();
	MY_WIFI.WIFI_Init();
	MY_WIFI.WIFI_Server_Init();
	OTA_Init(); //提供升级接口
	init_udp_time();
	scheduler_init();
}

// the loop function runs over and over again until power down or reset
void loop() {

	WiFi_Loop();
}