#include "infoFromNet.hpp"

#include <HTTPClient.h>
#include <M5EPD.h>
#include <WiFi.h>
#include <sys/time.h>
#include <time.h>

#include "util.h"

// wifiid.hには、ssid,passwordの各defineを定義を文字列として記載すること。
// このファイルは、.gitignoreとする。
#include "wifiid.h"
// apikey.hには、apikeyのdefineの定義を文字列として記載すること。
// このファイルは、.gitignoreとする。
#include "apikey.h"

GetInfoFromNetwork::GetInfoFromNetwork() { wifiOn(); }

GetInfoFromNetwork::~GetInfoFromNetwork() { wifiOff(); }

bool GetInfoFromNetwork::wifiOn(void) {
    WiFi.begin(ssid, password);
    for (int i = 0; i < 10; i++) {
        if (isWiFiOn()) return true;
        delay(500);
    }
    return false;
}

void GetInfoFromNetwork::wifiOff(void) {
    WiFi.disconnect(true);
    WiFi.mode(WIFI_OFF);
    return;
}

int GetInfoFromNetwork::isWiFiOn(void) {
    return (WiFi.status() == WL_CONNECTED);
}

int GetInfoFromNetwork::setNtpTime() {
    if (!isWiFiOn()) return -1;
    const long gmtOffset_sec = 0; 
    const int daylightOffset_sec = 0;
    const char* ntpServer = "jp.pool.ntp.org";

    struct timeval reset = {0, 0};
    settimeofday(&reset, NULL);
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    struct tm timeinfo;
    time_t t = time(NULL);
    if (t < 1000000000L) return -1;
    gmtime_r(&t, &timeinfo);

    rtc_time_t rtcTime;
    rtcTime.hour = (int8_t)timeinfo.tm_hour;
    rtcTime.min = (int8_t)timeinfo.tm_min;
    rtcTime.sec = (int8_t)timeinfo.tm_sec;
    rtc_date_t rtcDate;
    rtcDate.year = (int8_t)timeinfo.tm_year + 1900;
    rtcDate.mon = (int8_t)timeinfo.tm_mon + 1;
    rtcDate.day = (int8_t)timeinfo.tm_mday;
    M5.RTC.setDate(&rtcDate);
    M5.RTC.setTime(&rtcTime);

    return 0;
}

const uint8_t fingerprint[20] = {0xEE, 0xAA, 0x58, 0x6D, 0x4F, 0x1F, 0x42,
                                 0xF4, 0x18, 0x5B, 0x7F, 0xB0, 0xF2, 0x0A,
                                 0x4C, 0xDD, 0x97, 0x47, 0x7D, 0x99};
#define OpenWeatherUrl "api.openweathermap.org"
#define City "Nagahama,JP"

String GetInfoFromNetwork::getWeatherQuery() {
    String url("/data/2.5/forecast?");
    url += "q=" City;
    url += "&appid=" apikey;
    url += "&lang=ja&units=metric";
    return url;
}

// 天気予報データをSDカードのWeatherFileNameに書き込む。
bool GetInfoFromNetwork::getWeatherInfo() {
    if (!isWiFiOn()) return false;
    HTTPClient http;
    File file;
    String url = String("http://") + String(OpenWeatherUrl) + getWeatherQuery();
    Serial.print("URL:");
    Serial.println(url);
    if (!http.begin(url)) return false;
    int retCode = http.GET();
    if (retCode < 0) goto http_err;
    if (retCode != HTTP_CODE_OK && retCode != HTTP_CODE_MOVED_PERMANENTLY)
        goto http_err;
    if (!SD.exists("/")) goto http_err;
    Serial.println("SD OK!");
    if (SD.exists(WeatherFileName)) SD.remove(WeatherFileName);
    file = SD.open(WeatherFileName, FILE_WRITE);
    if (!file) goto http_err;
    Serial.println("ファイルオープン完了");
    if (http.writeToStream(&file) < 0) goto file_err;
    file.close();
    Serial.println("weatherファイルへのjsonデータ書き込み完了");
    http.end();
    return true;

file_err:
    file.close();
http_err:
    http.end();
    return false;
}
