// 天気予報の表示
#include <M5EPD.h>
#define LGFX_M5PAPER  
#include <LovyanGFX.hpp>

#include "tenki.hpp"

class DrawTenki {
    private:
        Tenki *tenki;
        LGFX_Sprite screen;
        int width;
        int height;
        float columnX[4];
        float rowCenterY[3];

    public:
        DrawTenki(Tenki *tenki, int width, int height);
        void draw(LovyanGFX *lcd, int x, int y) ;

    private:
        void drawFrameBorder() ;
        void calcColumnCoordinate();
        void drawTenkiInfo(int lineNo) ;

};
