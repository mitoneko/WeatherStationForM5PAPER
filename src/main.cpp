#include <M5EPD.h>
#define LGFX_M5PAPER  
#include <LovyanGFX.hpp>

#include "battery.h"
#include "thermometer.hpp"

static LGFX lcd;

void setup()
{
    M5.begin(false, true, true, true, true);
    M5.BatteryADCBegin();
    M5.RTC.begin();
    M5.SHT30.Begin();
    lcd.init();
    lcd.setRotation(1);
}


void loop()
{
    int bat = drawBattery(960-120-5, 5, &lcd);
    Thermometer t = Thermometer(350,350);
    t.drawString(10, 10, &lcd);
    t.drawTempMeter(87, 100, &lcd);
    t.drawHumMeter(524, 100, &lcd);

    if (bat > 90 || bat==1) {
        delay(5000);
    } else {
        delay(2000);
        M5.shutdown(58); // 一旦停止
    }
}
