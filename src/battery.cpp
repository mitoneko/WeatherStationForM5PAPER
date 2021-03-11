#include <M5EPD.h>

// バッテリー残量の取得
static int get_rest_battery() {
    const int max_vol = 4350;
    const int min_vol = 3300;
    //M5.BatteryADCBegin();
    int voltage = M5.getBatteryVoltage();
    voltage = max(voltage, min_vol);
    voltage = min(voltage, max_vol);
    float rest_battery_raw = (float)(voltage - min_vol) / (float)(max_vol - min_vol);
    rest_battery_raw = max(rest_battery_raw, 0.01f);
    rest_battery_raw = min(rest_battery_raw, 1.f);
    return (int)(rest_battery_raw * 100);
}

// バッテリー残量計の表示
int drawBattery(int x, int y) {
    M5EPD_Canvas *canvas = new M5EPD_Canvas(&M5.EPD);
    int rest_battery = get_rest_battery();
    
    // バッテリー矩形の表示
    canvas->createCanvas(120, 30); 
    canvas->fillCanvas(0);
    canvas->drawRect(10, 10, 45, 20, 15);
    canvas->fillRect(55, 17, 5, 5, 15);
    canvas->fillRect(10, 10, (int)((45*rest_battery)/100), 20, 15);

    // バッテリー残量文字の表示
    canvas->createRender(19, 20);
    canvas->setTextColor(15);
    canvas->setTextSize(19);
    canvas->setTextArea(62, 10, 60, 20);
    canvas->printf("%d%%", rest_battery);
    canvas->pushCanvas(x, y, UPDATE_MODE_GLD16);

    canvas->deleteCanvas();
    delete canvas;
    return rest_battery;
}

