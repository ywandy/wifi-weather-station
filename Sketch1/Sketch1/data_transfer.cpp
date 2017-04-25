#include "data_transfer.h"
extern TIMEDATA timedata;
DT mydt;
extern String comdata;
extern char req_dat[16];
extern int complete = 0;
int time_dat = 0;
extern char log_dat[16];
extern char time_iso[16];

void esp_send(float dat,char *s,char *tm){
    char str[86],flt[6];
    int code;
    HTTPClient http;
    dtostrf(dat,4,2,flt);
    //Serial.println(flt);
    sprintf(str,"{\"timestamp\":\"%s\",\"value\":%s}",tm,flt);
    //Serial.println(str);
    code = http.begin(s);
    http.addHeader("U-ApiKey","74c4601f0870e71ed6db8ec6f4741b33",1,1);
    code = http.sendRequest("POST",str);
    if(code==200){
        //Serial.println("success");
      }else{
       // Serial.print("failed! code:");
        //Serial.println(code);
        //Serial.print("cause:");
        //Serial.println(http.getString());              
      }
    //Serial.println(code);
    http.end();
    delay(100);
  }

int get_switch(char *url){
    int light_status = 0;
    HTTPClient http;
    int code = 0;
    String msg = "";
    code = http.begin(url);
    http.addHeader("U-ApiKey","74c4601f0870e71ed6db8ec6f4741b33",1,1);
    code = http.sendRequest("GET");
    if(code==200){
      //  Serial.println("success");
        msg = http.getString();
       // Serial.println(msg);     
        if(msg[msg.length()-2]=='0'){
          light_status = 0;
          }else if(msg[msg.length()-2]=='1'){
            light_status = 1;
            }
        return light_status;    
      }else{
      //  Serial.print("failed! code:");
       // Serial.println(code);
       // Serial.print("cause:");
         msg = http.getString();
        //Serial.println(msg);              
      }
   // Serial.println(code);
    http.end();
     return -1;     
  }


  void apart_num(){
    int sum = 0;
    int loop = 0;
    String inString = "";
	/*
	if (comdata[0] == 'f'){
			post_end(1); }*/
    while(comdata[loop]!='i'){
      if(loop == 6){
          break;
        }
       if(isDigit(comdata[loop])||comdata[loop]=='.'|| comdata[loop]=='-'){
          inString += comdata[loop];
        }
        loop++;
      }
      mydt.cur = inString.toFloat();
      inString = "";
      while(comdata[loop]!='v'){
      if(loop == 13){
          break;
        }
       if(isDigit(comdata[loop])||comdata[loop]=='.' ||comdata[loop] == '-'){
          inString += comdata[loop];
        }
        loop++;
      }
      mydt.vol = inString.toFloat();
      inString = "";
      while(comdata[loop]!='m'){
      if(loop == 19){
          break;
        }
       if(isDigit(comdata[loop])||comdata[loop]=='.'|| comdata[loop] == '-'){
          inString += comdata[loop];
        }
        loop++;
      }
      mydt.mh = inString.toFloat();
      inString = "";
      while(comdata[loop]!='p'){
      if(loop == 25){
          break;
        }
       if(isDigit(comdata[loop])||comdata[loop]=='.'|| comdata[loop] == '-'){
          inString += comdata[loop];
        }
        loop++;
      }
      mydt.power = inString.toFloat();
      inString = "";
      Serial.print("vol:");
      Serial.print(mydt.vol);
      Serial.print("cur:");
      Serial.print(mydt.cur);
      Serial.print("mh:");
      Serial.print(mydt.mh);
	  Serial.print("power:");
	  Serial.print(mydt.power);
	  Serial.print("\n");
  }

  int post_today_state(char *url) {
	  StaticJsonBuffer<200> jsonBuffer;
	  Serial.println(req_dat);
	  int code;
	  char str[80];
	  HTTPClient http;
	  sprintf(url, "%s/%s", url, req_dat);
	  code = http.begin(url);
	  http.addHeader("U-ApiKey", "74c4601f0870e71ed6db8ec6f4741b33", 1, 1);
	  code = http.sendRequest("GET");
	  if (code != 200) {
		  Serial.println("is not ok create the point!");
		  HTTPClient http2;
		  code = http2.begin("http://api.yeelink.net/v1.0/device/352479/sensor/397060/datapoints");
		  Serial.println(code);
		  http2.addHeader("U-ApiKey", "74c4601f0870e71ed6db8ec6f4741b33", 1, 1);
		  sprintf(str, "{\"key\":\"%s\",\"value\":{\"times\":\"0\"}}", req_dat);
		  Serial.println(str);
		  code = http2.sendRequest("POST", str);
		  http2.end();
		  return 1;
	  }
	  else if (code == 200) {
		  int loop = 0;
		  Serial.println("is ok!");
		  String msg = http.getString();
		  char str_json[50] = { 0 };
		  for (loop = 1; loop < msg.length() - 1; loop++) {
			  str_json[loop - 1] = msg[loop];
		  }//´¦ÀíÎªJSON
		  JsonObject& root = jsonBuffer.parseObject(str_json);
		  time_dat = root["value"]["times"];
		  Serial.print("today is ");
		  time_dat++;
		  Serial.println(time_dat);
		  HTTPClient http3;
		  //sprintf(url, "%s/%s", url, req_dat);
		  code = http3.begin(url);
		  http3.addHeader("U-ApiKey", "74c4601f0870e71ed6db8ec6f4741b33", 1, 1);
		  sprintf(str,"{\"value\":{\"times\":%d}}", time_dat);
		  code = http3.sendRequest("PUT", str);
		  http3.end();
		  return 0;
	  }
	  http.end();
  }
  
void post_json_data(long int start_time_stamp,long int end_time_stamp,int mode) {
	char str[120] = { 0 };
	HTTPClient http2;
	http2.begin("http://api.yeelink.net/v1.0/device/352479/sensor/397060/datapoints");
	http2.addHeader("U-ApiKey", "74c4601f0870e71ed6db8ec6f4741b33", 1, 1);
	sprintf(str, "{\"key\":\"%s%d\",\"value\":{\"mode\":%d,\"start\":%ld,\"end\":%ld}}",log_dat, time_dat, mode,start_time_stamp, end_time_stamp);
	Serial.println(str);
	int code = http2.sendRequest("POST", str);
	Serial.println(http2.getString());
	http2.end();
}
