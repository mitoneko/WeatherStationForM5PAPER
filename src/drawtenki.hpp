// 天気予報の表示
#include <M5EPD.h>
#define LGFX_M5PAPER  
#include <LovyanGFX.hpp>

#include "tenki.hpp"

class DrawTenki {
    private:
        Tenki *tenki;
        int width;
        int height;

    public:
        DrawTenki(Tenki *tenki, int width, int height);
        void draw(LovyanGFX *lcd, int x, int y) ;

    private:

};
