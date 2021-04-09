// メッセージエリアを作成し、表示管理する。
#include "messageArea.hpp"

MessageArea::MessageArea(int width, int height, int line, bool flame)
    : line_cnt(line), changed_text(false), changed_sprite(false), flame(flame) {
    if (line_cnt > MAX_LINE_CNT) line_cnt = MAX_LINE_CNT;
    mesg.setPsram(true);
    mesg.setColorDepth(4);
    mesg.createSprite(width, height);
    mesg.setFont(&fonts::lgfxJapanGothic_36);
    float line_height =
        (float)(height - 2 * (line_cnt - 1) - 2) / (float)line_cnt;
    float text_size = (float)line_height / (float)mesg.fontHeight();
    mesg.setTextSize(text_size);
    mesg.setTextColor(0, 15);
    for (int i = 0; i < MAX_LINE_CNT; i++) {
        line_buff[i][MAX_LINE_LENGTH] = '\0';
        line_buff[i][0] = '\0';
    }
}

// ソース文字列の内容をバッファにセットする。line_noは0始まり
// バッファの内容に変更があるかどうかをチェックし、必要がない場合書き換えない。
MessageArea *MessageArea::setText(const char *source, int line_no) {
    if (line_no >= line_cnt) line_no = line_cnt - 1;
    if (strcmp(source, line_buff[line_no]) == 0) return this;
    strncpy(line_buff[line_no], source, MAX_LINE_LENGTH);
    line_buff[line_no][MAX_LINE_LENGTH] = '\0';
    changed_text = true;
    return this;
};

// バッファの内容を必要があれば、スプライトに書き込む
MessageArea *MessageArea::flush(void) {
    if (changed_text) {
        int line_height = mesg.fontHeight();
        mesg.fillSprite(15);
        int width = mesg.width();
        int height = mesg.height();
        if (flame) mesg.drawRect(0, 0, width, height, 0);
        for (int i = 0; i < line_cnt; i++) {
            int y = line_height * i + 2 * i + 1;
            mesg.drawString(line_buff[i], 1, y);
        }
        changed_text = false;
        changed_sprite = true;
    }
    return this;
}

// スプライトに変更があれば、画面を描画する。
// forceが指定されれば、必ず描画する。
MessageArea *MessageArea::draw(LovyanGFX *lcd, int x, int y, bool force) {
    if (changed_sprite || force) {
        mesg.pushSprite(lcd, x, y);
        changed_sprite = false;
    }
    return this;
}

// 現在のバッファの内容を返す。line_noは0始まり
char *MessageArea::getText(char *buff, int buffSize, int line_no) {
    strncpy(buff, line_buff[line_no], buffSize);
    if (buffSize <= strlen(line_buff[line_no])) buff[buffSize] = '\0';
    return buff;
}
