#ifndef __NTPHANDLER_H
#define __NTPHANDLER_H
#include <time.h>                       // time() ctime()
#include <sys/time.h>                   // struct timeval
#include <sntp.h>                       // sntp_servermode_dhcp()
#include <TZ.h>


extern time_t currentNow;
time_t customNtp();

#endif //__NTPHANDLER_H
