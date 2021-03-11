#include <M5EPD.h>
#define LGFX_M5PAPER  
#include <LovyanGFX.hpp>

#include "battery.h"

static LGFX lcd;

void setup()
{
    M5.begin(false, true, true, true, true);
    M5.BatteryADCBegin();
    M5.RTC.begin();
    lcd.init();
    lcd.setRotation(1);


}


void loop()
{
    int bat = drawBattery(960-120-5, 5, &lcd);
    if (bat > 90 || bat==1) {
        delay(5000);
    } else {
        delay(2000);
        M5.shutdown(8); // 一旦停止
    }
}
