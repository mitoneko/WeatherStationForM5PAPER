#include <M5EPD.h>
#define LGFX_M5PAPER
#include <LovyanGFX.hpp>
#include <WiFi.h>
#include "infoFromNet.hpp"
#include "time.h"

// wifiid.hには、ssid,passwordの各defineを定義を記載すること。
// このファイルは、.gitignoreとする。
#include <wifiid.h>

GetInfoFromNetwork::GetInfoFromNetwork() {
    wifiOn();
}

GetInfoFromNetwork::~GetInfoFromNetwork() {
    wifiOff();
}

bool GetInfoFromNetwork::wifiOn(void) {
    WiFi.begin(ssid, password);
    for (int i = 0 ; i < 10; i++) {
        if (isWiFiOn()) return true;
        delay(500);
    }
    return false;
}

void GetInfoFromNetwork::wifiOff(void) {
    WiFi.disconnect(true);
    WiFi.mode(WIFI_OFF);
}

int GetInfoFromNetwork::isWiFiOn(void) {
    return (WiFi.status() == WL_CONNECTED) ;
}

int GetInfoFromNetwork::setNtpTime() {
    if (!isWiFiOn()) return -1;
    const long gmtOffset_sec = 9 * 3600;
    const int daylightOffset_sec = 0;
    const char * ntpServer = "jp.pool.ntp.org";
    
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) return -1;
    
    rtc_time_t rtcTime;
    rtcTime.hour = (int8_t)timeinfo.tm_hour;
    rtcTime.min = (int8_t)timeinfo.tm_min;
    rtcTime.sec = (int8_t)timeinfo.tm_sec ;
    rtc_date_t rtcDate ;
    rtcDate.year = (int8_t)timeinfo.tm_year + 1900;
    rtcDate.mon = (int8_t)timeinfo.tm_mon + 1;
    rtcDate.day = (int8_t)timeinfo.tm_mday ;
    M5.RTC.setDate(&rtcDate);
    M5.RTC.setTime(&rtcTime);
    return 0;
}
