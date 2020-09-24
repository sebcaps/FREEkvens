#ifndef __WEB_H
#define __WEB_H

extern String TimeZone ;
extern String NTPServer;


void initWebServer();
void webServerHandleClient();
void handleRoot();
void handleNotFound();
void handleSaveWifi();
void handleSaveTime();
void handleSaveOther();
void handleTools();
void handleReboot();
void handleUpdate();
void handleFSbrowser();
void handleReadfile();
void handleAPIWeb();
void handleTimeAPI();
void handleWifiAPI();
void handleOthAPI();
void handleSavePanel();
// void handleTimeSettings();
#endif