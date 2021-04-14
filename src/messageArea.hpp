// メッセージエリアを作成し、管理する。
#include <M5EPD.h>
#define LGFX_M5PAPER
#include <LovyanGFX.hpp>

#define MAX_LINE_CNT 5
#define MAX_LINE_LENGTH 80

class MessageArea {
   private:
    LGFX_Sprite mesg;
    int line_cnt;
    char line_buff[MAX_LINE_CNT][MAX_LINE_LENGTH + 1];
    bool changed_text;
    bool changed_sprite;
    bool flame;

   public:
    MessageArea(int width, int height, int line, bool flame = false);
    // ソース文字列の内容をバッファにセットする。line_noは0始まり
    MessageArea *setText(const char *source, int line_no);
    // バッファの内容を必要があれば、スプライトに書き込む
    MessageArea *flush(void);
    // スプライトに変更があれば、画面を描画する。
    // forceが指定されれば、必ず描画する。
    MessageArea *draw(LovyanGFX *lcd, int x, int y, bool force = false);
    // 現在のバッファの内容を返す。line_noは0始まり
    char *getText(char *buff, int buffSize, int line_no);

   private:
};
