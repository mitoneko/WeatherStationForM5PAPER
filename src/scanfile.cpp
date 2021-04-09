// Jpegファイルの一覧

#include "scanfile.hpp"

#include <M5EPD.h>
#include <SD.h>

JpegFiles::JpegFiles() : top(NULL), cur(NULL), _count(0) {
    if (!SD.exists("/")) return;
    File root = SD.open("/");
    File entry = root.openNextFile();
    while (entry) {
        if (!entry.isDirectory()) {
            if (isJpegFile(entry.name())) {
                addFilename(entry.name());
            }
        }
        entry = root.openNextFile();
    }
    entry.close();
    root.close();
}

FileNames *JpegFiles::addBlock(FileNames *last) {
    FileNames *addBlock = (FileNames *)ps_malloc(sizeof(FileNames));
    if (!addBlock) return NULL;
    if (last) last->next = addBlock;
    addBlock->prev = last;
    addBlock->next = NULL;
    return addBlock;
}

void JpegFiles::addFilename(const char *filename) {
    int itemInd = _count % blockSize;
    if (itemInd == 0) {
        cur = addBlock(cur);
        if (!top) top = cur;
    }
    strcpy(cur->filenames[itemInd], filename);
    _count++;
}

char *JpegFiles::operator[](int i) {
    int itemInd = i % blockSize;
    int blockNo = i / blockSize;

    if (i >= _count) return NULL;
    FileNames *block = top;
    for (int b = 0; b < blockNo; b++) {
        if (!block) return NULL;
        block = block->next;
    }
    return block->filenames[itemInd];
}

bool JpegFiles::isJpegFile(const char *filename) {
    const char *extBig = ".JPG";
    const char *extSmall = ".jpg";

    int check = 0;
    for (int i = 0; filename[i] != '\0'; i++) {
        if (filename[i] == extBig[check] || filename[i] == extSmall[check]) {
            check++;
        } else {
            if (check > 0 && (filename[i] == extBig[check] ||
                              filename[i] == extSmall[check])) {
                check = 1;
            } else {
                check = 0;
            }
        }
    }
    return extBig[check] == '\0';
}
