#include "WIFI_TCP.h"

WiFiClient client;
#define Bind_IP "192.168.1.132"
#define Bind_PORT 1760
void WIFI_TCP_Init() {
	int count = 2;
	//client.setNoDelay(1);
	client.setTimeout(100);
	while (!client.connected())
	{
		if (!client.connect(Bind_IP, Bind_PORT))
		{
			count--;
			delay(200);
			if (count == 0)
				break;
		}
	} //TCP连接
}

void Keep_Connection_Stable() {
	if (!client.connected()) {
		client.setTimeout(100);
		client.connect("192.168.1.132", 1760); //断线后尝试重新连接
	}
}

