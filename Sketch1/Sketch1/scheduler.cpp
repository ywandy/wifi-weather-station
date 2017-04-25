#include "scheduler.h"
//任务调度的文件
#include "ESP8266WiFi.h"
#include "ESP8266HTTPClient.h"
#include "ESP8266WebServer.h"
#include "WiFiUdp.h"
#include "WIFI_Connection.h"
#include "WIFI_OTA.h"
#include "data_transfer.h"
#include "net_time.h"
#define TEMP_SENSOR "http://api.yeelink.net/v1.0/device/357151/sensor/405136/datapoints"
extern WIFI_CONFIG MY_WIFI;
extern ESP8266WebServer server;
extern WiFiClient client;
extern char time_iso[16]; //时间戳全局
unsigned long int cnt_1ms, cnt_2ms, cnt_5ms, cnt_50ms, cnt_500ms, cnt_10ms,cnt_10s;
extern int led_state;
void scheduler_init()
{
	cnt_1ms = cnt_2ms = cnt_5ms = cnt_50ms = cnt_500ms = cnt_10ms = cnt_10s = 0;;
}

static void WIFI_Loop_10s(void) {
	time_get_net();
	Serial.println("Time got!");
	Serial.println(time_iso);
	esp_send(43.62, TEMP_SENSOR, time_iso);
}

static void WIFI_Loop_500ms(void)	//500ms执行一次
{
	digitalWrite(LED_BUILTIN, led_state);
	if (led_state == 0)
		led_state = 1;
	else
		led_state = 0;
	client.write("hello world");
}

static void WIFI_Loop_50ms(void)	//50ms执行一次
{
	//time_get_net();
}


static void WIFI_Loop_10ms(void)	//20ms执行一次
{
	ArduinoOTA.handle();
	server.handleClient(); //5ms响应一次http请求	
}


static void WIFI_Loop_5ms(void)	//5ms执行一次
{
	
}


void WiFi_Loop(void)
{

	if (millis() >= cnt_500ms)
	{
		cnt_500ms = millis() + 500;
		WIFI_Loop_500ms();
	}
	if (millis() >= cnt_50ms)
	{
		cnt_50ms = millis() + 50;
		WIFI_Loop_50ms();
	}
	if (millis() >= cnt_10ms)
	{
		cnt_10ms = millis() + 10;
		WIFI_Loop_10ms();
	}
	if (millis() >= cnt_5ms) 
	{
		cnt_5ms = millis() + 5;
		WIFI_Loop_5ms();
	}
	if (millis() >= cnt_10s) {
		WIFI_Loop_10s();
		cnt_10s = millis() + 10000;
	}

}

