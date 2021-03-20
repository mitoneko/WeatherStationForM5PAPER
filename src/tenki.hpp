// 天気予報のJSONの解析及び値の提供

#include <M5EPD.h>
#include <ArduinoJson.h>

class Tenki {
    private:
        DynamicJsonDocument json;
        DeserializationError jsonErr;
        bool _isEnable;


    public:
        Tenki();
        // 情報が有効か否かを返す。
        bool isEnable() { return _isEnable; }
        // リストナンバーの最大値を返す。
        int getMaxListNo() { return json["list"].size()-1; };
        // 現在時刻のh時間後以降の最小時刻のデータのリストNo.を取得する
        int getListIdAfterHHour(int h) ;
        // 指定リストナンバーの時刻を取得する。
        time_t getDate(int i) {
            time_t time = json["list"][i]["dt"].as<time_t>() + 9*3600;
            return time;
        };
        // 指定リストナンバーの天気を取得する。
        const char *getWeather(int i) {
            return json["list"][i]["weather"][0]["description"].as<const char *>();
        };
        // 指定リストナンバーの気温を取得する。
        float getTemp(int i) { return json["list"][i]["main"]["temp"].as<float>(); };
        // 指定リストナンバーの風向を取得する
        const char *getWindDir(int i) ;
        // 指定リストナンバーの風速を取得する。
        float getWindSpeed(int i) { return json["list"][i]["wind"]["speed"].as<float>(); };
        // 指定リストナンバーの風力階級を取得する
        int getBeaufortScale(int i) ;
        // 天気情報を最新のSDカードデータに更新する。
        void refresh() { _isEnable=false; readJson(); };

    private:
        void readJson() ;

};
