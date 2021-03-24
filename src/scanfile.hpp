// Jpegファイルの一覧を生成する。

#define blockSize 10
#define filenameSize 13 // 8+1+3+1(8.3形式に限る)

struct FileNames {
        char filenames[blockSize][filenameSize];
        FileNames *prev;
        FileNames *next;
};

class JpegFiles {
    private:
        Filenames *top;
        Filenames *cur;
        int count;
    public:
        JpegFiles();
        char *operator[](int i) ;

    private:
        FileNames *addBlock(FileNames *last) ;
        void addFilename(const char*filename) ;
        bool isJpegFile(const char* filename) {


};
