#include "ArduinoJson.h"
#include "FS.h"
#include "WIFI_Connection.h"
#include "stdio.h"
#include "stdlib.h"



int saveConfig();
void handleFileList();
void handleFileCreate();
void handleFileDelete();
void handleFileUpload();
bool handleFileRead(String path);
String getContentType(String filename);
String formatBytes(size_t bytes);

int loadConfig();
