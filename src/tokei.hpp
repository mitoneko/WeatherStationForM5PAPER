/**************************************************
 * 時計の表示
 **************************************************/

#include <M5EPD.h>
#define LGFX_M5PAPER
#include <LovyanGFX.hpp>

class Tokei {
   private:
    int year, month, day;
    int hour, min, sec;
    int dayOfTheWeek;
    int width, height;

    void getDateTime();
    int getDayOfTheWeek(int year, int month, int day);

   public:
    Tokei(int sizex = 200, int sizey = 200);
    void drawDigitalTokei(LovyanGFX *lcd, int x, int y);
};
