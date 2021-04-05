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
#include "scanfile.hpp"
#include "messageArea.hpp"

//#define MEMPRINT

static LGFX lcd;
bool startedOnTimer;
bool isOperateMode=false;
time_t timeOfOperateModeStart=0;
time_t timeOfLastUpdate=0;

inline void printMem(const char* msg) {
#ifdef MEMPRINT
    Serial.printf("【%s】heap:%'d, psram:%'d\n", msg, ESP.getFreeHeap(), ESP.getFreePsram());
#endif
}

void drawLcd() {
    drawBattery(960-120-5, 5, &lcd);
    printMem("バッテリー描画後のメモリ");

    printMem("時計描画前のメモリ");
    Tokei *tokei = new Tokei(300, 100);
    tokei->drawDigitalTokei(&lcd, 630, 50);
    printMem("時計描画後のメモリ");
    delete tokei;

    printMem("温度計描画前のメモリ");
    Thermometer *t = new Thermometer(200,200);
    t->drawTempMeter(&lcd, 530, 180);
    t->drawHumMeter(&lcd, 750, 180);
    printMem("温度計描画後のメモリ");
    delete t;

    printMem("お天気情報確保前のメモリ");
    Tenki *tenki = new Tenki();
    DrawTenki *drawTenki = new DrawTenki(tenki, 455, 112);
    drawTenki->draw(&lcd, 495, 408);
    printMem("お天気描画後のメモリ");
    delete drawTenki;
    delete tenki;

    //写真の表示。480*320がちょうど。
    //プログレッシブと最適化を無効にすること。
    //SDカードのルート直下のjpgファイルを対象とする。
    JpegFiles jpgs;
    char *filename = jpgs[random(jpgs.count())];
    Serial.print("写真番号:");
    Serial.println(filename);
    lcd.drawJpgFile(SD,filename, 10, 110);
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
    Serial.println("電源切ってみるテスト");
    int rest_sec = rest_minute()-4;
    if (rest_sec < 30) rest_sec += 60;
    M5.shutdown(rest_sec); // 一旦停止
    Serial.println("電源切れませんでした。");
}

void checkInfoFromNetwork(bool always=false) {
    rtc_time_t time;
    rtc_date_t date;
    M5.RTC.getDate(&date);
    M5.RTC.getTime(&time);
    time_t outdated = now() - 6*3600;
    Tenki tenki;

    if (!tenki.isEnable() || (time.hour%6==0 && time.min == 3) 
            || tenki.getDate(0)<outdated || date.year < 2020) {
        Serial.println("ネットワークの情報の取得開始");
        GetInfoFromNetwork info;
        info.setNtpTime();
        info.getWeatherInfo();
        tenki.refresh();
    }
}

// RTCモジュールのタイマーより起動されたかを確認する。
// その後、RTC.begin()相当の処理を実行する。
void saveStartedOnTimer() {
    Wire.begin(21,22);
    uint8_t rtcStatus = M5.RTC.readReg(0x01);
    startedOnTimer = (bool)(rtcStatus & 0x0c);
    M5.RTC.writeReg(0x00, 0x00);
    M5.RTC.writeReg(0x01, 0x00);
    M5.RTC.writeReg(0x0D, 0x00);
}

void setup()
{
    M5.begin(false, true, true, true, true);
    M5.BatteryADCBegin();
    M5.SHT30.Begin();
    SD.begin();
    lcd.init();
    lcd.setRotation(1);
    randomSeed(analogRead(0));
    saveStartedOnTimer();
    checkInfoFromNetwork();
    
    drawLcd();

    if (startedOnTimer) {
        challengeShutdown();
    }
    timeOfOperateModeStart = now();
    timeOfLastUpdate = timeOfOperateModeStart;
    isOperateMode = true;
}

MessageArea mesg(490, 50, 2, true);

void loop()
{
    M5.update();
    
    if (isOperateMode) {
        mesg.setText("操作可能", 0);
        mesg.flush();
        mesg.draw(&lcd, 5, 490);
    } else {
        mesg.setText("", 0);
        mesg.flush();
        mesg.draw(&lcd, 5, 490);
    }

    if (now() - timeOfOperateModeStart > 30) {
       isOperateMode = false;
    }

    int elapsed = now() - timeOfLastUpdate;
    if (rest_minute() > 55 && elapsed > 50 && !isOperateMode) {
        timeOfLastUpdate = now();
        checkInfoFromNetwork();
        drawLcd();
        challengeShutdown();
        isOperateMode = true;
        timeOfOperateModeStart = now();
    }
    delay(400);
}

