#include <M5EPD.h>
#define LGFX_M5PAPER  
#include <LovyanGFX.hpp>

#include "battery.h"
#include "thermometer.hpp"
#include "infoFromNet.hpp"
#include "tokei.hpp"
#include "tenki.hpp"
#include "drawtenki.hpp"
#include "util.h"

static LGFX lcd;

void psramtest2() {
    Serial.println(" ");
    if (psramFound()) {
        Serial.println("psramがありそう。");
    } else {
        Serial.println("psram無いのかな?");
    }
    Serial.println("psramの初期化も明示的にやってみよう。");
    bool ret = psramInit();
    if (ret) {
        Serial.println("出来た。または、もうやってた。");
    } else {
        Serial.println("失敗した。");
    }
    Serial.println("メモリ確保をこれからやります。");
    int *testmem = (int *)ps_malloc(sizeof(int)*2048);
    if (testmem) {
        Serial.println("確保できた");
    } else {
        Serial.println("確保失敗");
        return;
    }
    *(testmem+500) = 5216;
    Serial.println("addr500に5126を書いた");
    Serial.print("さて、addr500の値は・・・");
    Serial.println(testmem[500]);
}
    

void psramtest() {
    psramtest2();
    LGFX_Sprite test;
    Serial.println("sprite宣言");
    test.setColorDepth(4);
    Serial.println("カラー設定");
    test.setPsram(true);
    Serial.println("psram使用設定");
    test.createSprite(500,500);
    Serial.println("スプライト生成");
    test.fillSprite(0);
    Serial.println("スプライト塗りつぶし");
}


void drawLcd() {
    drawBattery(960-120-5, 5, &lcd);

    Tokei *tokei = new Tokei(300, 100);
    tokei->drawDigitalTokei(&lcd, 630, 50);
    delete tokei;

    Thermometer *t = new Thermometer(200,200);
    t->drawTempMeter(&lcd, 530, 180);
    t->drawHumMeter(&lcd, 750, 180);
    delete t;

    Tenki *tenki = new Tenki();
    DrawTenki *drawTenki = new DrawTenki(tenki, 455, 112);
    drawTenki->draw(&lcd, 495, 408);
    delete drawTenki;
    delete tenki;

    //写真の表示。480*320がちょうど。
    //プログレッシブと最適化を無効にすること。
    lcd.drawJpgFile(SD, "/photo001.jpg", 10, 110);

    psramtest();
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
    M5.RTC.getTime(&time);
    time_t outdated = now() - 6*3600;
    Tenki tenki;

    if (!tenki.isEnable() || (time.hour%6==0 && time.min == 3) || tenki.getDate(0)<outdated) {
        Serial.println("ネットワークの情報の取得開始");
        GetInfoFromNetwork info;
        info.setNtpTime();
        info.getWeatherInfo();
        tenki.refresh();
    }
}

void setup()
{
    M5.begin(false, true, true, true, true);
    M5.BatteryADCBegin();
    M5.RTC.begin();
    M5.SHT30.Begin();
    SD.begin();
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

