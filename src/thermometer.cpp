#include "thermometer.hpp"

Thermometer::Thermometer(int sizex, int sizey)
    : sizex(sizex), sizey(sizey) {
    M5.SHT30.Begin();
    radius = min(sizex, sizey)/2*0.95;
    makeScale();
    makeHand();
};

void Thermometer::makeScale() {
    scale[0].setColorDepth(4);
    scale[0].createSprite(radius/25, radius/5);
    scale[0].fillSprite(0);
    scale[0].setPivot(scale[0].width()/2, scale[0].height());
    scale[1].setColorDepth(4);
    scale[1].createSprite(radius/40, radius/7);
    scale[1].fillSprite(0);
    scale[1].setPivot(scale[1].width()/2, scale[1].height());
}

void Thermometer::makeHand() {
    float height, width;
    height = radius * 0.8f;
    width = height * 0.1f;

    hand.setColorDepth(4);
    hand.createSprite(width, height);
    hand.fillSprite(15);
    hand.setColor(0);
    hand.fillTriangle(width/2.f, 0, 0, height/4.f, width, height/4.f);
    hand.fillTriangle(0, height/4.f, width, height/4.f, width/2.f, height);
    hand.setPivot(width/2., height);
}

void Thermometer::makeMeterFace(int min, int max, const char* unit) {
    face.setColorDepth(4);
    face.createSprite(sizex, sizey);
    face.fillSprite(15);
    face.setColor(0);
    face.setFont(&fonts::lgfxJapanMinchoP_36);
    face.setTextColor(0, 15);
    face.setTextDatum(middle_center);
    float center[2] = {sizex/2.0f, sizey/2.0f};
    face.fillCircle(center[0], center[1], radius);
    face.fillCircle(center[0], center[1], radius*0.95, 15);
    float angleInterval = 270.f / (float)(max-min);
    for (int i = min ; i <= max ; i+=2) {
        LGFX_Sprite *use_scale = (i%10==0) ? &scale[0] : &scale[1];
        float angle = (270.f-45.f) - (float)(i-min) * angleInterval;
        float angleRad = angle * 3.14159265f / 180.f ;
        float startx = (radius - use_scale->height()) * cos(angleRad) + center[0];
        float starty = -1.0f * ((radius - use_scale->height()) * sin(angleRad)) + center[1];
        use_scale->pushRotateZoom(&face, startx, starty, 90.f-angle, 1.f, 1.f);
        if (i%10==0) {
            float charsize = (float)scale[0].height() / 36.f;
            float charx = (radius - use_scale->height() * 1.5f) * cos(angleRad) + center[0];
            float chary = -1.f * (radius - use_scale->height() * 1.5f) * sin(angleRad) + center[0];
            face.setTextSize(charsize);
            face.drawNumber(i, charx, chary);
        }
        face.drawString(unit, center[0], sizey/5.f*3.f);
    }

}

void Thermometer::drawTempMeter(int x, int y, LGFX *lcd) {
    makeMeterFace(0, 50, "℃");
    float center[2] = {(float)sizex/2.f, (float)sizey/2.f};
    float angle = 270.f - 45.f;
    angle -= 270.f / 50.f * get_temp();
    hand.pushRotateZoom(&face, center[0], center[1], 90.f - angle, 1.f, 1.f);
    face.pushSprite(lcd, x, y);
}

void Thermometer::drawHumMeter(int x, int y, LGFX *lcd) {
    makeMeterFace(20, 80, "%");
    float center[2] = {(float)sizex/2.f, (float)sizey/2.f};
    float angle = 270.f - 45.f;
    angle -= 270.f / 60.f * (get_hum() - 20.f);
    hand.pushRotateZoom(&face, center[0], center[1], 90.f - angle, 1.f, 1.f);
    face.pushSprite(lcd, x, y);
}

void Thermometer::drawString(int x, int y, LGFX *lcd) {
    M5.SHT30.UpdateData();
    LGFX_Sprite meter(lcd);
    meter.setColorDepth(4);
    meter.createSprite(250, 100);
    meter.fillSprite(15);
    meter.setColor(0);
    meter.setTextColor(0, 15);
    meter.setFont(&fonts::lgfxJapanMinchoP_36);
    meter.setCursor(10,10);
    meter.printf("温度:%5.1f℃", this->get_temp());
    meter.setCursor(10,50);
    meter.printf("湿度:%5.1f%%", this->get_hum());
    meter.pushSprite(x, y);
}

float Thermometer::get_temp() {
    M5.SHT30.UpdateData();
    this->temp = M5.SHT30.GetTemperature();
    return this->temp;
}

float Thermometer::get_hum() {
    M5.SHT30.UpdateData();
    this->hum = M5.SHT30.GetRelHumidity();
    return this->hum;
}
