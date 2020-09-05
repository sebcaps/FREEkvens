#ifndef __NTPHANDLER_H
#define __NTPHANDLER_H
#include "Arduino.h"
#include "Timezone.h"
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

// #include <TZ.h>
// #include <ESP8266WiFi.h>
// #include <coredecls.h> // settimeofday_cb()
// #include <Schedule.h>
// #include <PolledTimeout.h>
// #include <time.h>     // time() ctime()
// #include <sys/time.h> // struct timeval

const int NTP_PACKET_SIZE = 48;                                // NTP time is in the first 48 bytes of message
extern byte packetBuffer[];                     //buffer to hold incoming & outgoing packets
extern TimeChangeRule CEST ;
extern TimeChangeRule CET;   //Central European Time (Frankfurt, Paris)
extern Timezone CE;
extern TimeChangeRule *tcr;
extern time_t utc;
extern WiFiUDP Udp;

void sendNTPpacket(IPAddress &address);
time_t getNtpTime();

#endif //__NTPHANDLER_H
