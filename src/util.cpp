// ユーティリティー関数
#include <M5EPD.h>
#include <time.h>

time_t now() {
    struct tm time;
    rtc_time_t rtcTime;
    rtc_date_t rtcDate ;
    
    M5.RTC.getDate(&rtcDate);
    M5.RTC.getTime(&rtcTime);

    time.tm_year = rtcDate.year - 1900;
    time.tm_mon = rtcDate.mon - 1;
    time.tm_mday = rtcDate.day;
    time.tm_hour = rtcTime.hour;
    time.tm_min = rtcTime.min;
    time.tm_sec = rtcTime.sec;

    return mktime(&time);
}
