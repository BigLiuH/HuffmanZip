#ifndef ZIP_H
#define ZIP_H
#include <QProgressDialog>
#include <fstream>
typedef unsigned long long LLint;
/*
ZIP
压缩工具类
用于提供压缩、解压以及一些其他方法。
*/
class ZIP
{
public:
    static LLint encode(const char* srcPath, const char* dstPath, QProgressDialog* progress,std::map<int,std::string> &code,LLint w[256]); //压缩
    static void decode(const char *zipPath, const char *dstPath, QProgressDialog *progress); //解压
    static void dirdecode(const char* srcPath, const char* dstDirPath, QProgressDialog* progress);
    static void direncode(const char* srcDirPath, const char* dstPath, QProgressDialog* progress,std::map<int,std::string> &code,LLint w[256]);
    static char *getFileName(const char *path);                       //分离出文件名
    static char *getZipFileName(const char *path);                    //获取源文件名
    static bool checkZip(FILE *f);                                    //检查压缩文件合法性
    static bool UTF8ToUnicode(const char *UTF8, wchar_t *strUnicode); //字符转换
    static qint64 getFolderSize(const char* folderPath); //
private:
    ZIP();
};

#endif // ZIP_H
