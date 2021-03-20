// 天気予報JSONデータの解析　及び　値の提供

#include <M5EPD.h>
#include <time.h>

#include "infoFromNet.hpp"
#include "tenki.hpp"

Tenki::Tenki() : json(22528), _isEnable(false) {
    readJson();
}

void Tenki::readJson() {
    if (!SD.exists(WeatherFileName)) return ;
    File f = SD.open(WeatherFileName);
    if (!f) return;
    jsonErr = deserializeJson(json, f);
    if (jsonErr == DeserializationError::Ok) _isEnable=true;
    return;
}

// 指定リストナンバーの風力階級を取得する
int Tenki::getBeaufortScale(int i) {
    static const float speed_tbl[] =
        { 0.2, 1.5, 3.3, 5.4, 7.9, 10.7, 13.8, 17.1, 20.7, 24.4, 28.4, 32.6 };
    float speed = getWindSpeed(i);
    int scale = 0;
    while (scale < 12 && speed > speed_tbl[scale]) { scale++; }
    return scale;
}

// 指定リストナンバーの風向を取得する
// 内部文字列のポインターを返す。
const char *Tenki::getWindDir(int i) {
    static const char* dir[] = 
        {"北", "北東",  "東", "南東", "南", "南西", "西", "北西"}; 
    int deg = json["list"][i]["wind"]["deg"];
    int idx = ((2*deg+45)/45) % 8;  
    return dir[idx];
}

// 現在時刻のh時間後以降の最小時刻のデータのリストNo.を取得する
int Tenki::getListIdAfterHHour(int h) {
    rtc_date_t nowDate;
    rtc_time_t nowTime;
    M5.RTC.getTime(&nowTime);
    M5.RTC.getDate(&nowDate);

    struct tm nowTm;
    nowTm.tm_year=nowDate.year;
    nowTm.tm_mon=nowDate.mon-1;
    nowTm.tm_mday=nowDate.day;
    nowTm.tm_hour=nowTime.hour;
    nowTm.tm_min=nowTime.min;
    nowTm.tm_sec=nowTime.sec;
    time_t searchTime = mktime(&nowTm) + h * 3600;

    int listNo = 0;
    int max = getMaxListNo();
    while (listNo <= max &&  getDate(listNo) < searchTime) { listNo++; }
    return listNo;
}
