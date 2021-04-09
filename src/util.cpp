// ユーティリティー関数
#include <M5EPD.h>
#include <sys/time.h>
#include <time.h>

#define MinTimet 1000000000  // UNIX billennium 2001/9/9 01:46:40 UTC

void updateSystemTime() {
    struct tm setTime;
    struct timeval systime;
    rtc_time_t rtcTime;
    rtc_date_t rtcDate;

    M5.RTC.getDate(&rtcDate);
    M5.RTC.getTime(&rtcTime);

    setTime.tm_year = rtcDate.year - 1900;
    setTime.tm_mon = rtcDate.mon - 1;
    setTime.tm_mday = rtcDate.day;
    setTime.tm_hour = rtcTime.hour;
    setTime.tm_min = rtcTime.min;
    setTime.tm_sec = rtcTime.sec;

    systime.tv_sec = mktime(&setTime);
    systime.tv_usec = 0;

    settimeofday(&systime, NULL);
}

time_t now() {
    time_t t = time(NULL) ;
    if (t < MinTimet) {
        updateSystemTime();
        delay(10);
        t = time(NULL);
        if (t < MinTimet) t = 0;
    }
    return t;
}
