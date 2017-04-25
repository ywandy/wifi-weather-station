#include "WIFI_Connection.h"
WIFI_CONFIG MY_WIFI;
extern ESP8266WebServer server;
#define Retry_Count_Max 10
void handleRoot() { //80端口根
	String msg;
	if (server.hasArg("WIFI_NAME") && server.hasArg("WIFI_PASS")) {
		sprintf(MY_WIFI.wifiname, "%s", server.arg("WIFI_NAME").c_str());
		sprintf(MY_WIFI.wifipass, "%s", server.arg("WIFI_PASS").c_str());
		Serial.print("wifi-name:");
		Serial.println(MY_WIFI.wifiname);
		Serial.print("wifi-pass:");
		Serial.println(MY_WIFI.wifipass);
		server.send(200, "text/html", "<h1>Wifi config had been saved and esp is rebooting.....</h1>");
		delay(1000);
		WiFi.begin(MY_WIFI.wifiname, MY_WIFI.wifipass);
		if (saveConfig() == true) {
			Serial.println("Saved the data to flash! Is rebooting!!!");
			delay(1000);
			ESP.restart();
		}
	}
	String content = "<html><body><form action='/' method='POST'>Enter the wifi NAME && PASS<br>";
	content += "User:<input type='text' name='WIFI_NAME' placeholder='wifi_name'><br>";
	content += "Password:<input type='password' name='WIFI_PASS' placeholder='password'><br>";
	content += "<input type='submit' name='SUBMIT' value='Submit'></form>" + msg + "<br>";
	//content += "You also can go <a href='/inline'>here</a></body></html>";
	server.send(200, "text/html", content);
	// server.send(200, "text/html", "<h1>You are connected</h1>");
}


void WIFI_CONFIG::WIFI_Init()
{
	char str_buf[64];
	uint8_t macAddr[6];
	char mac_wifi[40];
	int count = 0;
	SPIFFS.begin();
	Dir dir = SPIFFS.openDir("/");
	while (dir.next()) {
		String fileName = dir.fileName();
		size_t fileSize = dir.fileSize();
		Serial.printf("FS File: %s, size: %s\n", fileName.c_str(), formatBytes(fileSize).c_str());
	}
	WiFi.mode(WIFI_AP_STA);
	WiFi.softAPmacAddress(macAddr);
	sprintf(mac_wifi, "ESP_WIFI:%02x:%02x:%02x:%02x:%02x:%02x", macAddr[0], macAddr[1], macAddr[2], macAddr[3], macAddr[4], macAddr[5]);
	WiFi.softAP(mac_wifi, "12345678"); //建立AP
	if (loadConfig() == true) { //如果文件读取成功，就尝试连接wifi
		Serial.println("flash read is success!");
		WiFi.begin(wifiname, wifipass);
		sprintf(str_buf, "Attempt to connect the %s %s", wifiname, wifipass);
		Serial.println(str_buf);
		while (WiFi.status() != WL_CONNECTED) {
			sprintf(str_buf, "%d connecting....", count++);
			Serial.println(str_buf);
			delay(500);
			if (count >= Retry_Count_Max) {
				Serial.println("Connect is failed,please open 192.168.4.1 to save the wifi name&pass");
				return;
			}
		}
		sprintf(str_buf, "Success to connect %s", wifiname);
		Serial.println(str_buf);
	}
}

void WIFI_CONFIG::WIFI_Server_Init()
{

	//SERVER INIT
	//list directory
	server.on("/list", HTTP_GET, handleFileList);
	//load editor
	server.on("/edit", HTTP_GET, []() {
		if (!handleFileRead("/edit.htm")) server.send(404, "text/plain", "FileNotFound");
	});
	//create file
	server.on("/edit", HTTP_PUT, handleFileCreate);
	//delete file
	server.on("/edit", HTTP_DELETE, handleFileDelete);
	//first callback is called after the request has ended with all parsed arguments
	//second callback handles file uploads at that location
	server.on("/edit", HTTP_POST, []() { server.send(200, "text/plain", ""); }, handleFileUpload);

	//called when the url is not defined here
	//use it to load content from SPIFFS
	server.onNotFound([]() {
		if (!handleFileRead(server.uri()))
			server.send(404, "text/plain", "FileNotFound");
	});

	//get heap status, analog input value and all GPIO statuses in one json call
	server.on("/all", HTTP_GET, []() {
		String json = "{";
		json += "\"heap\":" + String(ESP.getFreeHeap());
		json += ", \"analog\":" + String(analogRead(A0));
		json += ", \"gpio\":" + String((uint32_t)(((GPI | GPO) & 0xFFFF) | ((GP16I & 0x01) << 16)));
		json += "}";
		server.send(200, "text/json", json);
		json = String();
	});
	server.on("/", handleRoot);
	server.begin(); //启动路由设置服务器
}





