#include <M5EPD.h>
#define LGFX_M5PAPER  
#include <LovyanGFX.hpp>

#include "battery.h"
#include "thermometer.hpp"
#include "infoFromNet.hpp"
#include "tokei.hpp"

static LGFX lcd;

void drawLcd() {
    drawBattery(960-120-5, 5, &lcd);
    Thermometer t = Thermometer(200,200);
    t.drawTempMeter(&lcd, 500, 100);
    t.drawHumMeter(&lcd, 700, 100);

    Tokei tokei = Tokei(300, 100);
    tokei.drawDigitalTokei(&lcd, 100, 100);

    delay(500);
}

// ●分ピッタリまでの秒数
int rest_minute() {
    rtc_time_t time;
    M5.RTC.getTime(&time);
    return 60-time.sec;
}

// シャットダウンを試みる。通電中はすり抜ける
void challengeShutdown() {
    int rest_sec = rest_minute()-6;
    if (rest_sec < 30) rest_sec += 60;
    M5.shutdown(rest_sec); // 一旦停止
}

void checkInfoFromNetwork(bool always=false) {
    rtc_time_t time;
    rtc_date_t date;
    M5.RTC.getDate(&date);
    M5.RTC.getTime(&time);
    if ((time.hour%6==0 && time.min<1) || date.year<2020 || always) {
        Serial.println("ネットワークの情報の取得開始");
        GetInfoFromNetwork info;
        info.setNtpTime();
        info.getWeatherInfo();
    }
}

void setup()
{
    M5.begin(false, true, true, true, true);
    M5.BatteryADCBegin();
    M5.RTC.begin();
    M5.SHT30.Begin();
    SD.begin();
    Serial.begin(115200);
    lcd.init();
    lcd.setRotation(1);

    checkInfoFromNetwork();

    drawLcd();
    challengeShutdown();
}


void loop()
{
    delay((rest_minute()+1)*1000);
    checkInfoFromNetwork();
    drawLcd();
    challengeShutdown();
}

