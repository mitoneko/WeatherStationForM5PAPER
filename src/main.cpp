#include <M5EPD.h>
#define LGFX_M5PAPER
#include <LovyanGFX.hpp>

#include "battery.h"
#include "drawtenki.hpp"
#include "infoFromNet.hpp"
#include "messageArea.hpp"
#include "scanfile.hpp"
#include "tenki.hpp"
#include "thermometer.hpp"
#include "tokei.hpp"
#include "util.h"

//#define MEMPRINT

static LGFX lcd;
MessageArea *mesg;
bool startedOnTimer;
bool isOperateMode = false;
time_t timeOfOperateModeStart = 0;
time_t timeOfLastUpdate = 0;

inline void printMem(const char *msg) {
#ifdef MEMPRINT
    Serial.printf("【%s】heap:%'d, psram:%'d\n", msg, ESP.getFreeHeap(),
                  ESP.getFreePsram());
#endif
}

void drawLcd() {
    drawBattery(960 - 120 - 5, 5, &lcd);

    {
        Tokei tokei(300, 100);
        tokei.drawDigitalTokei(&lcd, 630, 50);
    }

    {
        Thermometer t(200, 200);
        t.drawTempMeter(&lcd, 530, 180);
        t.drawHumMeter(&lcd, 750, 180);
    }

    {
        Tenki tenki = Tenki();
        DrawTenki drawTenki(&tenki, 455, 112);
        drawTenki.draw(&lcd, 495, 408);
    }

    //写真の表示。480*320がちょうど。
    //プログレッシブと最適化を無効にすること。
    // SDカードのルート直下のjpgファイルを対象とする。
    JpegFiles jpgs;
    char *filename = jpgs[random(jpgs.count())];
    Serial.print("写真番号:");
    Serial.println(filename);
    lcd.drawJpgFile(SD, filename, 10, 110);
    delay(400);
}

// ●分ピッタリまでの秒数
int rest_minute() {
    time_t time = now();
    int sec = (int)(time % 60);
    return 60 - sec;
}

// シャットダウンを試みる。通電中はすり抜ける
void challengeShutdown() {
    Serial.println("電源切ってみるテスト");
    int rest_sec = rest_minute() - 4;
    if (rest_sec < 30) rest_sec += 60;
    M5.shutdown(rest_sec);  // 一旦停止
    Serial.println("電源切れませんでした。");
}

void checkInfoFromNetwork(bool always = false) {
    time_t timenow = now();
    time_t outdated = timenow - 10 * 60;
    Tenki tenki;

    if (!tenki.isEnable() || tenki.getDate(0) < outdated || timenow == 0) {
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
    Wire.begin(21, 22);
    uint8_t rtcStatus = M5.RTC.readReg(0x01);
    startedOnTimer = (rtcStatus & 0x0c) != 0 ? true : false;
    M5.RTC.writeReg(0x00, 0x00);
    M5.RTC.writeReg(0x01, 0x00);
    M5.RTC.writeReg(0x0D, 0x00);
}

void setup() {
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

    mesg = new MessageArea(490, 50, 2, true);
}

void loop() {
    M5.update();

    if (isOperateMode) {
        char buff[80];
        if (strlen(mesg->getText(buff, 80, 0)) == 0) {
            strcat(buff, "操作可能->");
        }
        if (strlen(buff) > 78) {
            strcpy(buff, "操作可能->");
        }
        if (M5.BtnR.wasPressed()) {
            strcat(buff, "R");
            timeOfOperateModeStart = now();
        }
        if (M5.BtnL.wasPressed()) {
            strcat(buff, "L");
            timeOfOperateModeStart = now();
        }
        if (M5.BtnP.wasPressed()) {
            strcat(buff, "P");
            timeOfOperateModeStart = now();
        }
        mesg->setText(buff, 0)->flush()->draw(&lcd, 5, 490);
    } else {
        mesg->setText("", 0)->flush()->draw(&lcd, 5, 490);
    }

    isOperateMode = (now() - timeOfOperateModeStart < 30);

    int elapsed = now() - timeOfLastUpdate;
    if (rest_minute() > 55 && elapsed > 50) {
        timeOfLastUpdate = now();
        checkInfoFromNetwork();
        drawLcd();
        if (!isOperateMode) {
            challengeShutdown();
            isOperateMode = true;
            timeOfOperateModeStart = now();
        }
    }
    delay(400);
}
