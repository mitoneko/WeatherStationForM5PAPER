// Jpegファイルの一覧を生成する。

#define blockSize 10
#define filenameSize 14  // 1+8+1+3+1(8.3形式に限る。'/'が付加される)

struct FileNames {
    char filenames[blockSize][filenameSize];
    FileNames *prev;
    FileNames *next;
};

class JpegFiles {
   private:
    FileNames *top;
    FileNames *cur;
    int _count;

   public:
    JpegFiles();
    char *operator[](int i);
    int count() { return _count; };

   private:
    FileNames *addBlock(FileNames *last);
    void addFilename(const char *filename);
    bool isJpegFile(const char *filename);
};
