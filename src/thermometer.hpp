#include <M5EPD.h>
#define LGFX_M5PAPER  
#include <LovyanGFX.hpp>

class Thermometer {
    private:
        float temp;
        float hum;
        int sizex;
        int sizey;
        float radius;
        LGFX_Sprite face;
        LGFX_Sprite scale[2];
        LGFX_Sprite hand;

        void makeMeterFace(int min, int max, const char* unit);
        void makeScale();
        void makeHand(); 
    public:
        Thermometer(int sizex=200, int sizey=200);

        float get_temp();
        float get_hum();

        void drawTempMeter(int x, int y, LGFX *lcd);
        void drawHumMeter(int x, int y, LGFX *lcd);
        void drawString(int x, int y, LGFX *lcd);
};
