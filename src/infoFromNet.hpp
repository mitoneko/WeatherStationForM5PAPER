// ネットワークより取得する情報関連
// 時計・天気予報
#include <M5EPD.h>
#define LGFX_M5PAPER
#include <LovyanGFX.hpp>

class GetInfoFromNetwork {
    private:
        
        int wifiOn(void);
        void wifiOff(void);

    public:
        GetInfoFromNetwork();
        int setNtpTime() ;
        void printWifiStatus(LGFX *lcd, int x, int y) ;
        void printDateTime(LGFX *lcd, int x, int y) ;
};        
