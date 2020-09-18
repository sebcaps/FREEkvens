#ifndef __WEB_H
#define __WEB_H

extern String TimeZone ;
extern String NTPServer;


void initWebServer();
void webServerHandleClient();
void handleRoot();
void handleNotFound();
void handleSaveConfig();
void handleTools();
void handleReboot();
void handleUpdate();
void handleFSbrowser();
void handleReadfile();
void handleAPIWeb();
void handleAPI();
void handleSavePanel();
void handleTimeSettings();
#endif