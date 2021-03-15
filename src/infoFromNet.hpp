// ネットワークより取得する情報関連
// 時計・天気予報
#include <M5EPD.h>
#define LGFX_M5PAPER
#include <LovyanGFX.hpp>

class GetInfoFromNetwork {
    private:

        bool wifiOn(void);
        void wifiOff(void);
    public:
        GetInfoFromNetwork();
        ~GetInfoFromNetwork() ;
        int isWiFiOn(void);
        int setNtpTime() ;
};        
