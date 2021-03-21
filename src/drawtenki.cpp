// 天気予報の表示
#include <M5EPD.h>
#define LGFX_M5PAPER  
#include <LovyanGFX.hpp>
#include <time.h>

#include "drawtenki.hpp"

DrawTenki::DrawTenki(Tenki *tenki, int width, int height)
    : tenki(tenki), width(width), height(height)  {

}

void DrawTenki::draw(LovyanGFX *lcd, int x, int y) {
    LGFX_Sprite sprite;
    sprite.setColorDepth(4);
    sprite.createSprite(width, height);
    sprite.fillSprite(15);
    sprite.setTextColor(0, 15);
    sprite.setColor(0);
    sprite.setFont(&fonts::lgfxJapanGothic_36);
    float textsize = (height / 4) / 36;
    sprite.drawRect(0, 0, width, height);
    for (int i=1; i <= 3 ; i++) {
        sprite.drawLine(0, height/4*i, width, height/4*i);
    }
    sprite.setTextSize(textsize);
    for (int i=0; i < 1 ; i++) {
        int top = (height / 3) * i + 1;
        int listNo = 4;
        time_t dataTimet = tenki->getDate(listNo);
        struct tm *dataTm = gmtime(&dataTimet);
        char textbuf[100];
        sprintf(textbuf, "%2d日%2d時", dataTm->tm_mday, dataTm->tm_hour);
        int x = 1;
        sprite.drawString(textbuf, x, top);
        x += sprite.textWidth(textbuf) + 5;
        sprite.drawString(tenki->getWeather(listNo), x, top);
        x += sprite.textWidth("激しい雨") + 5;
        sprintf(textbuf, "%2d℃", (int)tenki->getTemp(listNo));
        sprite.drawString(textbuf, x, top);
        x += sprite.textWidth("00℃") + 5;
        sprite.drawString(tenki->getWindDir(listNo), x, top);
        x += sprite.textWidth("南東") + 5;
        sprite.drawNumber(tenki->getBeaufortScale(listNo), x, top);
    }
    sprite.pushSprite(lcd, x, y);
}
