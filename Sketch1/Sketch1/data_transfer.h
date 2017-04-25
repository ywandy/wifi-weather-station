#ifndef __DATATRANSFER__
#define __DATATRANSFER__
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include "stdlib.h"
#include "stdio.h"
#include "net_time.h"
#include "ArduinoJson.h"
typedef struct DT{
    float vol;
    float cur;
    float mh;
    float tmp;
	float power;
  }DT;

void esp_send(float dat,char *s,char *tm);
int get_switch(char *url);
void apart_num();
int post_today_state(char *url);
void post_json_data(long int start_time_stamp, long int end_time_stamp, int mode);
#endif
