// Jpegファイルの一覧

#include <SD.h>

#include "scanfile.hpp"

JpegFiles::JpegFiles() : count(0), top(NULL), cur(NULL) {
    if (!SD.exists("/")) return ;
    File entry = SD.oepn("/");
    while (entry) {
        if (entry.isDirectory()) continue;
        if (isJpegFile(entry.name()) addFilename(entry.name());
        entry = openNextFile();
    }
}

FileNames *JpegFiles::addBlock(FileNames *last) {
    FileNames *addBlock= (FileNames*)ps_malloc(sizeof(FileNames));
    if (!addBlock) return NULL;
    if (last) last->next = addBlock;
    addBlock->prev = last;
    addBlock->next = NULL;
    return addBlock;
}

void JpegFiles::addFilename(const char *filename) {
    int itemInd =  count % blockSize;
    if (itemInd == 0) {
        cur = addBlock(cur);
        if (!top) top = cur;
    }
    strcpy(cur->filenames[itemInd], filename);
    count ++;
}

char *JpegFiles::operator[](int i) {
    int itemInd = i % blockSize;
    int blockNo = i / blockSize;

    FileNames *block = top;
    for (int b=0; b < blockNo; b++) {
        if (!block) return NULL;
        block = block->next;
    }
    return block->filenames[itemInd];
}

bool JpegFiles::isJpegFile(const char* filename) {
    const char* extBig = ".JPG";
    const char* extSmall = ".jpg";

    int check=0;
    for (int i=0; filename[i]!='\0'; i++) {
        if (filename[i] == extBig[check] || filename[i] == extSmall[check]) {
            check ++;
        } else {
            if (check > 0 && (filename[i] == extBig[check] || filename[i] == extSmall[check])) {
                check = 1;
            } else {
                check = 0;
            }
        }
    }
    return extBig[check] == '\0';
}
