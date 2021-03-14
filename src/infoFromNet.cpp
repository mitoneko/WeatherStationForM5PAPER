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
}

int GetInfoFromNetwork::setNtpTime() {
    wifiOn();
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
    wifiOff();
    return 0;
}
    
int GetInfoFromNetwork::wifiOn(void) {
    WiFi.begin(ssid, password);
    for (int i = 0 ; i < 10; i++) {
        if (WiFi.status() == WL_CONNECTED) return 0;
        delay(500);
    }
    return -1;
}

void GetInfoFromNetwork::wifiOff(void) {
    WiFi.disconnect(true);
    WiFi.mode(WIFI_OFF);
}

// デバッグ用
void GetInfoFromNetwork::printWifiStatus(LGFX *lcd, int x, int y) {
    LGFX_Sprite s(lcd);
    s.setColorDepth(4);
    s.createSprite(100,30);
    s.fillSprite(15);
    s.setFont(&fonts::lgfxJapanMinchoP_20);
    s.setTextColor(0, 15);
    s.setCursor(1,1);
    s.printf("wifi:%s", (WiFi.status() == WL_CONNECTED) ? "on" : "off");
    s.pushSprite(x, y);
}

void GetInfoFromNetwork::printDateTime(LGFX *lcd, int x, int y) {
    LGFX_Sprite s(lcd);
    s.setColorDepth(4);
    s.createSprite(300,80);
    s.fillSprite(15);
    s.setFont(&fonts::lgfxJapanMinchoP_32);
    s.setTextColor(0, 15);
    s.setCursor(1,1);
    rtc_time_t rtcTime;
    rtc_date_t rtcDate;
    M5.RTC.getTime(&rtcTime);
    M5.RTC.getDate(&rtcDate);
    s.printf("%4d年%2d月%2d日\n%2d:%2d:%2d", 
            rtcDate.year, rtcDate.mon, rtcDate.day,
            rtcTime.hour, rtcTime.min, rtcTime.sec);
    s.pushSprite(x, y);
}
