#include "zip.h"
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <huffmantree.h>
#include <string.h>
#include <map>
#include <deque>
#include <QDebug>
#include <windows.h>
#include <QProgressDialog>
#include <QCoreApplication>
#include <ctime>
#include <QFile>
#include <QDir>
#include <QDataStream>
#include <filesystem>

using namespace std;

ZIP::ZIP()
{
    //构造函数为空，主要是为了保持类的静态性质。
}

/*
压缩
将目标文件压缩到指定路径
char* srcPath: 被压缩的文件路径
char* dstPath: 压缩到的文件路径
*/
LLint ZIP::encode(const char* srcPath, const char* dstPath, QProgressDialog* progress,map<int,string> &code,LLint w[256])//压缩函数
{
    /*
    srcPath: 被压缩的文件路径。
    dstPath: 压缩后文件的存储路径。
    progress: QProgressDialog，用于显示进度
    */
    /*
    实现步骤:

    路径转换: 使用 UTF8ToUnicode 将路径从 UTF-8 转换为宽字符（Unicode）。
    文件打开: 使用 _wfopen 打开源文件和目标文件。
    文件长度获取: 获取源文件的长度，用于设置进度条范围。
    文件名处理: 提取文件名并检查其长度。
    字符频率统计: 读取源文件中的每个字符，统计每个字符的出现次数，构建频率数组。
    哈夫曼树构建: 使用频率数组构建哈夫曼树。
    数据编码: 使用哈夫曼编码压缩源文件的数据，将编码后的二进制数据写入目标文件。
    文件关闭: 关闭源文件和目标文件。
    */
    /*
    关键变量:
    wSrcPath, wdstPath: 宽字符路径。
    fileLen: 文件长度。
    weights: 统计字符出现次数的数组。
    */
    wchar_t wSrcPath[2048],wdstPath[2048];
    /*
    路径转换： 使用 UTF8ToUnicode 函数将 UTF-8 编码的路径转换为 Unicode（wchar_t 类型）。
    该步骤是为了确保路径能够在 Windows 系统下正确处理，因为 Windows 文件操作函数 _wfopen 使用的是宽字符（wchar_t）。
    */
    //将源路径以及解压路径都转换为UTF8
    if(!UTF8ToUnicode(srcPath, wSrcPath) || !UTF8ToUnicode(dstPath, wdstPath))
        throw runtime_error("路径转换字符集失败！");
    //文件打开： 使用 _wfopen 打开源文件和目标压缩文件。如果打开失败，抛出异常并终止程序。
    FILE *fin = _wfopen(wSrcPath,L"rb");//被压文件
    FILE *fout = _wfopen(wdstPath,L"wb");//压缩后的文件
    QString exeDirPath = QCoreApplication::applicationDirPath();  // 获取程序目录
    QString filePath ="F:/QTProject/HuffmanZip/history.txt";
    QFile hisfile(filePath);

    if(!fin)
    {
        qDebug()<<"压缩失败！不能打开被压文件！";
        throw runtime_error("不能打开被压文件！\n请检查选择文件路径");
    }
    else if(!fout)
    {
        qDebug()<<"压缩失败！不能打开目标文件！";
        throw runtime_error("不能打开目标文件！\n请检查压缩到路径");
    }

    /*
    获取文件大小：通过 _fseeki64(fin, 0, SEEK_END) 将文件指针移动到文件末尾。
    存储文件大小：调用 fgetpos(fin, &fileLen)，将文件指针的位置（即文件大小）存储到 fileLen 中。
    此时，fileLen 保存的是从文件开始到文件末尾的字节数，也就是文件的总大小。
    */

    //进度条

    fpos_t fileLen;//文件大小（字节）// 定义一个文件位置结构体变量，用于存储文件指针的位置
    _fseeki64(fin,0,SEEK_END);   // 将文件指针移动到文件末尾
    fgetpos(fin, &fileLen);      // 获取当前文件指针的位置，即文件的大小// 获取当前文件指针的位置，并将其存储在fileLen中
    _fseeki64(fin,0,SEEK_SET);   // 将文件指针重新移动到文件开头
    int steps = 0;//进度条计数器
    long long each = fileLen/50+1;//设计每次更新的步长（+1防止步长为0）
    long long circleTimes = 0;//记录文件处理的循环次数
    progress->setRange(0,100);//设置进度条
    //获取文件名： 调用 getFileName 函数，从 srcPath 中提取出文件名（包括扩展名）。
    //文件名长度不能超过 256 字节，否则会抛出异常。
    char* fileName = getFileName(srcPath);//获取文件名
    int fileNameLen = strlen(fileName);//文件名长度
    if(fileNameLen >= 256)//文件名长度不能超过256
    {
        qDebug()<<"压缩失败！文件【"<<fileName<<"】文件名过长！";
        throw runtime_error("源文件文件名过长！");
    }
    LLint count=0;  // 作为占位符的初始值
    long long pos = ftell(fout);  // 记录当前位置
    fwrite(&count, sizeof(LLint), 1, fout);
    // 保存当前位置，作为占位符的位置

    fputc(fileNameLen,fout);//将文件名长度写入压缩文件中
    fputs(fileName,fout);//将文件名也写入到压缩文件中

    //建立哈夫曼树
    int c;
    LLint weights[256];
    memset(weights,0,sizeof(weights));//置0
    while (true) {
        c = fgetc(fin);//逐字符读取文件
        if(feof(fin)) break;//到达文件末尾停下来

        //防止界面假死
        if((++circleTimes&65535)==65535)//进行65535次循环后，处理界面事件
            QCoreApplication::processEvents();
        //更新进度条
        if(circleTimes==each)// 每次循环处理一个字节
        {
            QCoreApplication::processEvents();//界面响应
            circleTimes = 0;
            progress->setValue(++steps);
        }

        //统计字符出现
        weights[c]++;//含隐式转换
    }
    HuffmanTree tree(weights);//构建一哈夫曼树

    for(int i=0;i<256;i++)
    {
        w[i]=weights[i];
    }

    circleTimes = 0;
    //将权值数组写入压缩文件中
    fwrite(&weights, sizeof(weights[0]), 256, fout);

    {//压缩文件数据
        map<int,string> codeTable = tree.getCodeTable();//获取编码表
        code=codeTable;
        _fseeki64(fin,0,SEEK_SET);//重新开始读取被压文件//开始逐字符压缩（编码转换）
        //重新定位fin的位置
        string codes = "";//待压缩的二进制数据，用string临时存储
        /*register*/int len = 0;//用于存储codes长度
        while (true) {
            c = fgetc(fin);

            if(feof(fin)) break;//文件结束 退出
            codes += codeTable[c];//通过编码表获得编码//注意这是一个子符串
            len = codes.length();
            while(len>=8)//每满8个二进制位就存储一次
            {
                //将二进制转为0-255之间的数字
                /*register */int temp = 0;
                for(int i=0;i<8;i++)//转换为数字
                {
                    temp<<=1;
                    temp |= (codes[i]-'0');
                }
                //字符串缩短
                codes = codes.substr(8,len-8);
                len -= 8;
                //将压缩后的数据写入
                fputc(temp,fout);
                count++;//记录写入的字符数
            }

            //防止界面假死，更新进度条
            if((++circleTimes&65535)==65535)
                QCoreApplication::processEvents();
            if(circleTimes==each)
            {
                QCoreApplication::processEvents();
                circleTimes = 0;
                progress->setValue(++steps);
            }
        }
        //写入最后不满8位的数据，用0补齐
        int temp = 0;
        for(int i=0;i<len;i++)
        {
            temp<<=1;
            temp |= (codes[i]-'0');
        }
        for(int i=0;i<8-len;i++)
            temp<<=1;
        fputc(temp,fout);
        count++;
        //写入最后剩余的位数
        fputc(len,fout);
        count++;
        fseek(fout, pos, SEEK_SET);  // 定位到占位符的位置
        fwrite(&count, sizeof(LLint), 1, fout);
        qDebug()<<"数目"<<count;
    }
    if (hisfile.open(QIODevice::Append | QIODevice::Text)) {
        hisfile.write(dstPath);
        hisfile.write("\n");
        hisfile.close();  // 关闭文件
        qDebug() << "Content appended successfully!";
    } else {
        qDebug() << "Failed to open the file!";
    }
    hisfile.close();
    //关闭文件
    fclose(fin);
    fclose(fout);

    //进度条补全
    for(; steps<=100; steps++) progress->setValue(steps),QCoreApplication::processEvents();;

    //释放内存
    delete[] fileName;

    return count;
}

/*
解压
char* zipPath: 压缩文件路径
char* dstDir: 解压的目标文件夹
char* fileNam: 文件名
*/
void ZIP::decode(const char* zipPath, const char* dstPath, QProgressDialog* progress) {
    wchar_t wZipPath[2048], wdstPath[2048];
    if (!UTF8ToUnicode(zipPath, wZipPath) || !UTF8ToUnicode(dstPath, wdstPath))
        throw runtime_error("路径转换字符集失败！");
    progress->setRange(0,100);//设置进度条
    FILE *fin = _wfopen(wZipPath, L"rb");  // 压缩文件
    FILE *fout = _wfopen(wdstPath, L"wb"); // 解压后的文件

    if (!fin) {
        qDebug() << "解压失败！不能打开压缩文件！";
        throw runtime_error("不能打开压缩文件！\n请检查压缩文件路径！");
    } else if (!fout) {
        qDebug() << "解压失败！不能打开目标解压文件！";
        throw runtime_error("不能打开目标解压文件！\n请检查解压到路径！");
    }
    LLint zipcount=0;
    fread(&zipcount,sizeof(LLint),1,fin);
    qDebug()<<"zipcount"<<zipcount;
    LLint step=zipcount/100+1;

    int fileNameLen = fgetc(fin);
    _fseeki64(fin, fileNameLen, SEEK_CUR); // 跳过文件名部分

    // 读取哈夫曼树的权重数组
    LLint count=0;
    LLint weights[256];
    fread(weights, sizeof(weights[0]), 256, fin);

    HuffmanTree tree(weights);
    map<int, string> codeTable = tree.getCodeTable();

    // 解压数据部分
    deque<char> codeQueue;
    int c1, c2 = fgetc(fin);
    count++;
    char c;
    HuffmanTreeNode* root = tree.getRoot();
    HuffmanTreeNode* now = root;

    // 获取压缩数据并解码
    long long fileLen = 0;
    LLint prostep=0;
    while (true) {
        prostep++;
        while (codeQueue.size() > 0) {
            c = codeQueue.front();
            codeQueue.pop_front();
            if (c == '0') now = now->lchild;
            else if (c == '1') now = now->rchild;

            if (now == nullptr) throw runtime_error("哈夫曼树解码失败！");
            if (now->data != -1) {
                fputc(now->data, fout);  // 找到叶子节点并写入数据
                now = root;
            }
        }

        // 读取下一个字节并转换为二进制
        c1 = c2;
        count++;
        //if(!feof(fin)) count++;
        c2 = fgetc(fin);

        if (count>zipcount) {
            for (int i = 0; i < 8 - c1; i++)
                codeQueue.pop_back();
            break;
        }

        // 每次8位转换为一个字节的二进制数据
        for (int i = 7; i >= 0; i--) {
            if (c1 & (1 << i)) codeQueue.push_back('1');
            else codeQueue.push_back('0');
        }

        // 更新进度条
        if(prostep==step)
        {
            progress->setValue(++fileLen);
            QCoreApplication::processEvents();
            prostep=0;
        }

    }
    for(;fileLen<=100;)
    {
        progress->setValue(++fileLen);
        QCoreApplication::processEvents();
    }
    qDebug()<<"数目"<<count;
    fclose(fin);
    fclose(fout);
}


/*
分离出文件名（包括扩展名）
char* path: 文件路径
return: char* 文件名
*/
char* ZIP::getFileName(const char* path)
{
    int idx=-1,len=strlen(path);
    for(int i=0;i<len;i++)//获取最后的文件夹分隔符
    {
        if(path[i]=='/' || path[i]=='\\')
            idx = i;
    }
    char *fileName = new char[len-idx+1];
    if(idx!=-1)//若有分隔符，则切割
    {
        strcpy(fileName,path+idx+1);
    }
    else
    {
        strcpy(fileName,path);
    }
    return fileName;
}

void ZIP::direncode(const char* srcDirPath, const char* dstPath,QProgressDialog* progress,std::map<int,std::string> &code,LLint w[256]){
    wchar_t wdstPath[2048];
    if(!UTF8ToUnicode(dstPath, wdstPath))
        throw runtime_error("路径转换字符集失败！");
    FILE *fout = _wfopen(wdstPath, L"wb");//压缩后的文件
    if(!fout)
    {
        qDebug()<<"压缩失败！不能打开目标文件！";
        throw runtime_error("不能打开目标文件！\n请检查压缩到路径");
    }

    QString dirPath(srcDirPath);
    QFile fileout(srcDirPath);
    qint64 fileoutSize = fileout.size();
    fileout.close();
    long long each = fileoutSize/50+1;//设计每次更新的步长（+1防止步长为0）
    QDir dir(dirPath);
    int filecount=0;// 文件数目
    // 获取目录下所有文件
    QFileInfoList files = dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot); // 获取所有文件，不包括 "." 和 ".."
    LLint weights[256]={0};
    progress->setRange(0,100);
    long long step=0;
    // 遍历所有文件
    for (const QFileInfo &fileInfo : files) {
        // 更新进度条
        progress->setValue(++step);
        QCoreApplication::processEvents();
        QFile file(fileInfo.absoluteFilePath());

        // 打开文件
        if (!file.open(QIODevice::ReadOnly)) {
            qWarning() << "无法打开文件：" << fileInfo.absoluteFilePath();
            continue;
        }

        // 逐字符读取文件
        QByteArray fileData = file.readAll();  // 读取所有文件数据
        for (char c : fileData) {
            // 统计字符频率
            unsigned char uc = static_cast<unsigned char>(c);
            weights[uc]++;
        }
        filecount++;
        file.close();
    }

    for(int i=0;i<256;i++)
    {
        w[i]=weights[i];
    }
    HuffmanTree tree(weights);//构建一哈夫曼树

    map<int,string> codeTable = tree.getCodeTable();//获取编码表
    code=codeTable;
    // 写入文件数目
    fwrite(&filecount,sizeof(int),1,fout);
    //将权值数组写入压缩文件中
    fwrite(&weights, sizeof(weights[0]), 256, fout);

    for (const QFileInfo& fileInfo : files) {
        // 更新进度条
        progress->setValue(++step);
        QCoreApplication::processEvents();
        QString fileName = fileInfo.fileName();  // 获取文件名

        QByteArray byteArray = fileName.toUtf8();
        const char* cFileName = byteArray.constData();
        qDebug() << "文件名 (char*):" << cFileName;
        int length=strlen(srcDirPath)+strlen(cFileName)+2;
        char *filepath=(char*)malloc(length);
        strcpy(filepath,srcDirPath);
        strcat(filepath,"/");
        strcat(filepath,cFileName);
        qDebug()<<filepath;
        char *fileph=new char[256];
        int i;
        for(i=0;i<strlen(cFileName);i++)
        {
            fileph[i]=cFileName[i];
        }
        fileph[i]='\0';
        wchar_t wsrcPath[2048];
        if(!UTF8ToUnicode(filepath, wsrcPath))
            throw runtime_error("路径转换字符集失败！");
        FILE *fin = _wfopen(wsrcPath, L"rb");//压缩后的文件
        if(!fin)
        {
            qDebug()<<"压缩失败！不能打开源文件！";
            throw runtime_error("不能打开源文件！\n请检查源文件路径");
        }
        LLint count=0;  // 作为占位符的初始值
        long long pos = ftell(fout);  // 记录当前位置
        fwrite(&count, sizeof(LLint), 1, fout);
        int fileNameLen=strlen(cFileName);
        fputc(fileNameLen,fout);//将文件名长度写入压缩文件中
        fputs(cFileName,fout);//将文件名也写入到压缩文件中
        // fwrite(fileph,sizeof(char),256,fout);

        count=0;
        //_fseeki64(fin,0,SEEK_SET);//重新开始读取被压文件//开始逐字符压缩（编码转换）
        //重新定位fin的位置
        int c;
        string codes = "";//待压缩的二进制数据，用string临时存储
        /*register*/int len = 0;//用于存储codes长度
        while (true) {
            c = fgetc(fin);

            if(feof(fin)) break;//文件结束 退出
            codes += codeTable[c];//通过编码表获得编码//注意这是一个子符串
            len = codes.length();
            while(len>=8)//每满8个二进制位就存储一次
            {
                //将二进制转为0-255之间的数字
                /*register */int temp = 0;
                for(int i=0;i<8;i++)//转换为数字
                {
                    temp<<=1;
                    temp |= (codes[i]-'0');
                }
                //字符串缩短
                codes = codes.substr(8,len-8);
                len -= 8;
                //将压缩后的数据写入
                fputc(temp,fout);
                count++;
            }

        }
        //写入最后不满8位的数据，用0补齐
        int temp = 0;
        for(int i=0;i<len;i++)
        {
            temp<<=1;
            temp |= (codes[i]-'0');
        }
        for(int i=0;i<8-len;i++)
            temp<<=1;
        fputc(temp,fout);
        count++;
        //写入最后剩余的位数
        fputc(len,fout);
        count++;
        fseek(fout, pos, SEEK_SET);  // 定位到占位符的位置
        fwrite(&count, sizeof(LLint), 1, fout);
        // 移动文件指针到文件末尾
        fseek(fout, 0, SEEK_END);  // 将文件指针移动到文件末尾
        qDebug()<<"数目"<<count;
        fclose(fin);
        delete [] filepath;
    }
    for(;step<=100;)
    {
        progress->setValue(++step);
        QCoreApplication::processEvents();
    }

    fclose(fout);
}

void ZIP::dirdecode(const char* srcPath, const char* dstDirPath, QProgressDialog* progress) {
    // 转换路径为宽字符
    wchar_t wZipPath[2048];
    if (!UTF8ToUnicode(srcPath, wZipPath))
        throw runtime_error("路径转换字符集失败！");

    // 打开压缩文件
    FILE *fin = _wfopen(wZipPath, L"rb");  // 压缩文件
    if (!fin) {
        qDebug() << "无法打开压缩文件：" << wZipPath;
        throw runtime_error("无法打开压缩文件！");
    }

    // 读取文件数目
    int filecount = 0;
    fread(&filecount, sizeof(int), 1, fin);
    qDebug() << "文件数目" << filecount;
    progress->setRange(0,filecount);
    // 读取权值数组
    LLint weights[256] = {0};
    fread(&weights, sizeof(weights[0]), 256, fin);
    HuffmanTree tree(weights);
    map<int, string> codeTable = tree.getCodeTable();
    LLint fileLen=0;
    for (int i = 0; i < filecount; i++) {
        // 更新进度条
        progress->setValue(++fileLen);
        QCoreApplication::processEvents();
        LLint zipcount = 0;
        fread(&zipcount, sizeof(LLint), 1, fin);
        qDebug() << zipcount;
        LLint count=0;
        int filenamelength = fgetc(fin);  // 读取文件名的长度（单个字节）
        if (filenamelength <= 0) {
            qDebug() << "文件名长度无效！";
            continue;  // 如果文件名长度无效，跳过当前文件
        }

        // 分配内存存储文件名
        char* filename = new char[filenamelength + 1];  // +1 用于存储 '\0' 结束符
        if (!filename) {
            throw std::runtime_error("内存分配失败！");
        }

        // // 读取文件名字符
        fread(filename, sizeof(char), filenamelength, fin);
        filename[filenamelength] = '\0';  // 确保字符串正确结束

        qDebug() << "文件名长度：" << filenamelength;
        qDebug() << "文件名：" << filename;

        //构造完整路径
        size_t pathLength = strlen(dstDirPath) + strlen(filename) + 3;  // +2 为路径分隔符和字符串结束符
        char* filepath = new char[pathLength];
        if (!filepath) {
            throw std::runtime_error("内存分配失败！");
        }

        strcpy(filepath, dstDirPath);
        if (dstDirPath[strlen(dstDirPath) - 1] != '/') {
            strcat(filepath, "/");
        }
        strcat(filepath, filename);

        qDebug() << "完整文件路径：" << filepath;
        // if(strstr(filepath,".doc")||strstr(filepath,".txt")||strstr(filepath,".docx")) zipcount--;
        if(strstr(filepath,".bmp")) zipcount++;
        // zipcount--;
        // 打开目标文件以写入解压数据
        wchar_t wdstPath[2048];
        if (!UTF8ToUnicode(filepath, wdstPath))
            throw runtime_error("路径转换字符集失败！");

        FILE *fout = _wfopen(wdstPath, L"wb");  // 使用 "wb" 模式
        if (!fout) {
            qDebug() << "无法打开输出文件：" << wdstPath;
            throw runtime_error("无法打开输出文件！");
        }

        // 解压数据部分
        deque<char> codeQueue;

        int c1,c2 = fgetc(fin);
        count++;
        char c;
        HuffmanTreeNode* root = tree.getRoot();
        HuffmanTreeNode* now = root;


        while (true) {
            while (codeQueue.size() > 0) {
                c = codeQueue.front();
                codeQueue.pop_front();
                if (c == '0') now = now->lchild;
                else if (c == '1') now = now->rchild;

                if (now == nullptr) {
                    throw runtime_error("哈夫曼树解码失败！路径无效。");
                }

                if (now->data != -1) {
                    fputc(now->data, fout);  // 找到叶子节点并写入数据
                    now = root;
                }
            }

            // 读取下一个字节并转换为二进制
            c1 = c2;
            count++;
            //if(!feof(fin)) count++;
            c2 = fgetc(fin);


            if (count>=zipcount) {
                for (int i = 0; i < 8 - c1; i++)
                    codeQueue.pop_back();
                break;
            }

            // 每次8位转换为一个字节的二进制数据
            for (int i = 7; i >= 0; i--) {
                if (c1 & (1 << i)) codeQueue.push_back('1');
                else codeQueue.push_back('0');
            }

        }
        if(strstr(filename,".bmp")) fseek(fin, -1, SEEK_CUR);  // 将文件指针从当前位置向后移动1个字节
        // if(strstr(filename,".bmp")) fputc('c',fout);
        // 释放内存
        delete[] filename;
        delete[] filepath;
        fclose(fout);
    }

    fclose(fin);

}


/*
获取源文件名
char* path: 压缩文件路径
return: char* 原文件名
*/
char *ZIP::getZipFileName(const char* path){
    wchar_t wPath[2048];
    UTF8ToUnicode(path, wPath);
    FILE *fin = _wfopen(wPath, L"rb");//打开压缩文件
    if(!checkZip(fin)) throw runtime_error("压缩文件可能损坏");
    LLint count=0;
    fread(&count,sizeof(LLint),1,fin);
    qDebug()<<count;
    int fileNameLen = fgetc(fin);//获取文件名长度
    char* fileName = new char[fileNameLen+2];//用于储存文件名
    fgets(fileName,fileNameLen+1,fin);//获取被压文件名
    if(ferror(fin)) throw runtime_error("文件读取失败");
    fclose(fin);
    return fileName;
}

bool ZIP::checkZip(FILE *f) {
    if (f == nullptr) {
        return false; // 文件为空
    }

    // 检查尾部字节
    if (_fseeki64(f, -1, SEEK_END) != 0) {
        return false; // 无法移动指针
    }
    int t = fgetc(f);
    if (t == EOF || t < 0 || t >= 256) {
        return false; // 尾部字节超出范围
    }

    // 检查头部字节
    if (_fseeki64(f, 0, SEEK_SET) != 0) {
        return false; // 无法移动指针
    }
    t = fgetc(f);
    if (t == EOF || t <= 0 || t >= 256) {
        return false; // 头部字节超出范围
    }

    // 重置文件指针
    _fseeki64(f, 0, SEEK_SET);
    return true; // 文件有效
}

/*
工具函数，转换字符
char *UTF8: uft8字符串
wchar_t *strUnicode: 存储wchar的数组
return: 是否成功
*/
bool ZIP::UTF8ToUnicode(const char *UTF8, wchar_t *strUnicode)
{
    DWORD dwUnicodeLen;    //转换后Unicode的长度
    TCHAR*pwText;      //保存Unicode的指针
    //获得转换后的长度，并分配内存
    dwUnicodeLen = MultiByteToWideChar(CP_UTF8,0,UTF8,-1,NULL,0);
    pwText = new TCHAR[dwUnicodeLen];
    if(!pwText) return false;
    //转为Unicode
    MultiByteToWideChar(CP_UTF8,0,UTF8,-1,pwText,dwUnicodeLen);
    //转为CString
    wcscpy(strUnicode, pwText);
    //清除内存
    delete[]pwText;
    return true;
}
// 获取文件夹文件大小
qint64 ZIP::getFolderSize(const char* folderPath) {
    QDir dir(folderPath);
    if (!dir.exists()) {
        qDebug() << "文件夹不存在:" << folderPath;
        return -1;  // 返回错误值
    }

    qint64 totalSize = 0;

    // 获取文件夹中的所有文件（不包括 . 和 ..）
    QFileInfoList fileList = dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot);
    for (const QFileInfo& fileInfo : fileList) {
        // 累加文件的大小
        totalSize += fileInfo.size();
    }

    // // 遍历子文件夹并递归计算大小
    // QFileInfoList dirList = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
    // for (const QFileInfo& dirInfo : dirList) {
    //     if (dirInfo.isDir()) {
    //         totalSize += getFolderSize(dirInfo.absoluteFilePath().toStdString().c_str());
    //     }
    // }

    return totalSize;
}
