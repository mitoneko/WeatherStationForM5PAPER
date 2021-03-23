/**************************************************
 * 時計の表示
 **************************************************/

#include <M5EPD.h>
#define LGFX_M5PAPER  
#include <LovyanGFX.hpp>

#include "tokei.hpp"

Tokei::Tokei(int width, int height) 
        : width(width), height(height) {
    getDateTime();
    dayOfTheWeek = getDayOfTheWeek(year, month, day);
}

// RTCより現在時刻を取得する。
void Tokei::getDateTime() {
    rtc_time_t time;
    rtc_date_t date;

    M5.RTC.getTime(&time);
    M5.RTC.getDate(&date);

    year = date.year;
    month = date.mon;
    day = date.day;
    hour = time.hour;
    min = time.min;
    sec = time.sec;
}

// 曜日の計算。月曜日を0、日曜日を6とする。
int Tokei::getDayOfTheWeek(int year, int month, int day) {
    int y = year % 100;
    int c = y / 100;
    int ganma = 5 * c + c / 4;
    return (day+(26*(month+1))/10+y+y/4+ganma+5)%7;
}

// デジタル時計を描画する
void Tokei::drawDigitalTokei(LovyanGFX *lcd, int x, int y) {
    // 描画領域区分比率
    const float tokei_ratio = 0.75f;
    // スプライト初期化
    LGFX_Sprite tokei;
    tokei.setPsram(true);
    tokei.setColorDepth(4);
    tokei.createSprite(width,height);
    tokei.fillSprite(15);
    //tokei.drawRect(0,0,width,height,0); // レイアウト検討用外枠
    // 時計時間部分表示
    char strTime[6];
    sprintf(strTime, "%02d:%02d", hour, min);
    tokei.setFont(&fonts::Font7); // font高さ:48
    tokei.setTextColor(0,15);
    float mag = (height*tokei_ratio) / 48.f;
    tokei.setTextSize(mag, mag);
    tokei.drawString(strTime, 0.f, height*(1-tokei_ratio));
    // 時計日時部分表示
    const char* youbi_tbl[] = 
        { "月", "火", "水", "木", "金", "土", "日" };
    const char* youbi = youbi_tbl[dayOfTheWeek];
    char strDate[30];
    sprintf(strDate, "%d年%2d月%2d日(%s)", year, month, day, youbi);
    tokei.setFont(&fonts::lgfxJapanGothic_36);
    mag = (height*(1-tokei_ratio)) / 36.f;
    tokei.setTextSize(mag, mag);
    tokei.drawString(strDate, 0.f, 0.f);
    tokei.pushSprite(lcd, x, y);
}

