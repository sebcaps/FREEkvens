#include "webUtils.h"
const char *NTPServerList[5] = {"pool.ntp.org", "asia.pool.ntp.org", "europe.pool.ntp.org", "oceania.pool.ntp.org", "south-america.pool.ntp.org"};
const char *getTimeZone(byte tz)
{
    switch (tz)
    {
    case Europe_Paris:
        return TZ_Europe_Paris;
        break;
    case Europe_London:
        return TZ_Europe_London;
        break;
    case Africa_Niame:
        return TZ_Africa_Niamey;
        break;
    case America_New_York:
        return TZ_America_New_York;
        break;
    case Asia_Seoul:
        return TZ_Asia_Seoul;
        break;
    default:
        return TZ_Etc_UTC;
        break;
    }
};