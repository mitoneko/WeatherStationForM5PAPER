// 天気予報の表示
#include <M5EPD.h>
#define LGFX_M5PAPER  
#include <LovyanGFX.hpp>
#include <time.h>

#include "drawtenki.hpp"

DrawTenki::DrawTenki(Tenki *tenki, int width, int height)
    : tenki(tenki), width(width), height(height)  {
    screen.setColorDepth(4);
    screen.createSprite(width, height);
    screen.fillSprite(15);
    screen.setColor(0);
    screen.setTextColor(0,15);
    screen.setFont(&fonts::lgfxJapanGothic_36);
    float textsize = (((float)height-10.f) / 3.f) / 36.f;
    screen.setTextSize(textsize);
    calcColumnCoordinate();
}

// 各カラムの水平垂直座標の計算
void DrawTenki::calcColumnCoordinate() {
    columnX[0] = 2.f;
    columnX[1] = columnX[0] + screen.textWidth("88日88時") + 3;
    columnX[2] = columnX[1] + screen.textWidth("激しい雨") + 3;
    columnX[3] = columnX[2] + screen.textWidth("00℃") + 3;
    rowCenterY[0] = height/6.f;
    for (int i = 1; i<=2; i++) {
        rowCenterY[i] = rowCenterY[i-1] + height/3.f;
    }
}

// フレーム枠の表示
void DrawTenki::drawFrameBorder() {
    screen.drawRect(0, 0, width, height);
    for (int i=1; i<=2; i++) {
        screen.drawLine(0, height/3*i, width, height/3*i);
    }
    for (int i=1; i<=3; i++) {
        screen.drawLine(columnX[i]-1, 0, columnX[i]-2, height);
    }
}


// 一行の天気情報を描画する。 lineno:0-2
void DrawTenki::drawTenkiInfo(int lineNo) {
    const int hour[3] = { 12, 24, 48 };
    screen.setTextDatum(middle_left);
    int listNo = tenki->getListIdAfterHHour(hour[lineNo]);

    //時間
    time_t dataTimet = tenki->getDate(listNo);
    struct tm *dataTm = gmtime(&dataTimet);
    char textbuf[100];
    sprintf(textbuf, "%2d日%2d時", dataTm->tm_mday, dataTm->tm_hour);
    screen.drawString(textbuf, columnX[0], rowCenterY[lineNo]);
    //天気
    screen.drawString(tenki->getWeather(listNo), columnX[1], rowCenterY[lineNo]);
    //気温
    sprintf(textbuf, "%2d℃", (int)tenki->getTemp(listNo));
    screen.drawString(textbuf, columnX[2], rowCenterY[lineNo]);
    //風向、風力
    const char *windDir = tenki->getWindDir(listNo);
    int beaufortScale = tenki->getBeaufortScale(listNo);
    sprintf(textbuf, "%s %d", windDir, beaufortScale);
    screen.drawString(textbuf, columnX[3], rowCenterY[lineNo]);
}

void DrawTenki::draw(LovyanGFX *lcd, int x, int y) {
    drawFrameBorder();
    for (int i=0; i < 3 ; i++) {
        drawTenkiInfo(i);
    }
    screen.pushSprite(lcd, x, y);
}
