#include <M5EPD.h>

#include "battery.h"

//M5EPD_Canvas canvas(&M5.EPD);

void setup()
{
    M5.begin(false, true, true, true, true);
    M5.BatteryADCBegin();
    M5.RTC.begin();
    M5.EPD.Clear(true);
    M5EPD_Canvas *_init_canvas = new M5EPD_Canvas(&M5.EPD);
    _init_canvas->loadFont("/font.ttf", SD);
}


void loop()
{
    int bat = drawBattery(960-120-5, 5);
    if (bat > 90 || bat==1) {
        delay(5000);
    } else {
        delay(2000);
        M5.shutdown(8); // 一旦停止
    }
}
