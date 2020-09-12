#ifndef __WEB_H
#define __WEB_H

extern String text ;
extern int sizeText;
extern int scrollText;
extern int xText;
extern int yText;
extern int light;

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

#endif