#include "mainwindow.h"
#include <QMessageBox>
#include <QProgressDialog>
#include "ui_mainwindow.h"
#include "zip.h"
#include <cstring>
#include <string.h>
#include <thread>
#include <QStringListModel>
#include <QListView>
#include <QFileDialog>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>


typedef unsigned long long LLint;
#include<QFile>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    //设置窗口图标
    // setWindowIcon(QIcon(QStringLiteral(":/new/prefix1/myicon.png")));
    setWindowIcon(QIcon());

    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
    //Line Edit样式（QSS样式表）
    ui->leComAim->setStyleSheet("QLineEdit{border-width:1px;border-radius:4px;font-size:12px;color:"
                                "black;border:1px solid gray;}"
                                "QLineEdit:hover{border-width:1px;border-radius:4px;font-size:12px;"
                                "color:black;border:1px solid rgb(70,200,50);}");
    ui->leComChose->setStyleSheet("QLineEdit{border-width:1px;border-radius:4px;font-size:12px;"
                                  "color:black;border:1px solid gray;}"
                                  "QLineEdit:hover{border-width:1px;border-radius:4px;font-size:"
                                  "12px;color:black;border:1px solid rgb(70,200,50);}");
    ui->leUnAim->setStyleSheet("QLineEdit{border-width:1px;border-radius:4px;font-size:12px;color:"
                               "black;border:1px solid gray;}"
                               "QLineEdit:hover{border-width:1px;border-radius:4px;font-size:12px;"
                               "color:black;border:1px solid rgb(70,200,50);}");
    ui->leUnChose->setStyleSheet("QLineEdit{border-width:1px;border-radius:4px;font-size:12px;"
                                 "color:black;border:1px solid gray;}"
                                 "QLineEdit:hover{border-width:1px;border-radius:4px;font-size:"
                                 "12px;color:black;border:1px solid rgb(70,200,50);}");
    ui->leUnname->setStyleSheet("QLineEdit{border-width:1px;border-radius:4px;font-size:12px;color:"
                                "black;border:1px solid gray;}"
                                "QLineEdit:hover{border-width:1px;border-radius:4px;font-size:12px;"
                                "color:black;border:1px solid rgb(70,200,50);}");

    //PushButton 样式
    ui->btnComChose->setStyleSheet(
        "QPushButton{background-color:gray;color: white;   border-radius: 10px;  border: 2px "
        "groove gray;border-style: outset;}"
        "QPushButton:hover{background-color:white; color: gray;}"
        "QPushButton:pressed{background-color:rgb(70, 200, 50); border-style: inset;}");
    ui->btnComAim->setStyleSheet(
        "QPushButton{background-color:gray;color: white;   border-radius: 10px;  border: 2px "
        "groove gray;border-style: outset;}"
        "QPushButton:hover{background-color:white; color: gray;}"
        "QPushButton:pressed{background-color:rgb(70, 200, 50); border-style: inset;}");
    ui->btnComsure->setStyleSheet(
        "QPushButton{background-color:gray;color: white;   border-radius: 10px;  border: 2px "
        "groove gray;border-style: outset;}"
        "QPushButton:hover{background-color:white; color: gray;}"
        "QPushButton:pressed{background-color:rgb(70, 200, 50); border-style: inset;}");
    ui->btnComCancel->setStyleSheet(
        "QPushButton{background-color:gray;color: white;   border-radius: 10px;  border: 2px "
        "groove gray;border-style: outset;}"
        "QPushButton:hover{background-color:white; color: gray;}"
        "QPushButton:pressed{background-color:rgb(70, 200, 50); border-style: inset;}");
    ui->btnUnAim->setStyleSheet(
        "QPushButton{background-color:gray;color: white;   border-radius: 10px;  border: 2px "
        "groove gray;border-style: outset;}"
        "QPushButton:hover{background-color:white; color: gray;}"
        "QPushButton:pressed{background-color:rgb(70, 200, 50); border-style: inset;}");
    ui->btnUnCancel->setStyleSheet(
        "QPushButton{background-color:gray;color: white;   border-radius: 10px;  border: 2px "
        "groove gray;border-style: outset;}"
        "QPushButton:hover{background-color:white; color: gray;}"
        "QPushButton:pressed{background-color:rgb(70, 200, 50); border-style: inset;}");
    ui->btnUnChose->setStyleSheet(
        "QPushButton{background-color:gray;color: white;   border-radius: 10px;  border: 2px "
        "groove gray;border-style: outset;}"
        "QPushButton:hover{background-color:white; color: gray;}"
        "QPushButton:pressed{background-color:rgb(70, 200, 50); border-style: inset;}");
    ui->btnUnSure->setStyleSheet(
        "QPushButton{background-color:gray;color: white;   border-radius: 10px;  border: 2px "
        "groove gray;border-style: outset;}"
        "QPushButton:hover{background-color:white; color: gray;}"
        "QPushButton:pressed{background-color:rgb(70, 200, 50); border-style: inset;}");
    ui->btnUnname->setStyleSheet("background-color:gray;color: white;   border-radius: 10px;  "
                                 "border: 2px groove gray;border-style: outset;");

    //设置Line Edit阴影效果
    QGraphicsDropShadowEffect *effect1 = new QGraphicsDropShadowEffect();
    QGraphicsDropShadowEffect *effect2 = new QGraphicsDropShadowEffect();
    QGraphicsDropShadowEffect *effect3 = new QGraphicsDropShadowEffect();
    QGraphicsDropShadowEffect *effect4 = new QGraphicsDropShadowEffect();
    QGraphicsDropShadowEffect *effect5 = new QGraphicsDropShadowEffect();
    effect1->setOffset(2, 2);
    effect2->setOffset(2, 2);
    effect3->setOffset(2, 2);
    effect4->setOffset(2, 2);
    effect5->setOffset(2, 2);
    ui->leComAim->setGraphicsEffect(effect1);
    ui->leComChose->setGraphicsEffect(effect2);
    ui->leUnAim->setGraphicsEffect(effect3);
    ui->leUnChose->setGraphicsEffect(effect4);
    ui->leUnname->setGraphicsEffect(effect5);
}
QString MainWindow::getDirectoryFromPath(const QString &filePath) {
    QFileInfo fileInfo(filePath);
    QDir dir = fileInfo.dir();  // 获取目录信息
    return dir.absolutePath();  // 返回绝对路径
}

MainWindow::~MainWindow()
{
    delete ui;
}

//压缩函数
void MainWindow::ziphan(QString aim,QString chose)
{
    //判断文件情况
    QFileInfo unzip(chose), zip(aim);
    if (!unzip.isFile()) {
        QMessageBox::warning(this, "压缩失败", "选择文件不存在！");
        return;
    } else if (zip.isFile()) {
        QMessageBox message(QMessageBox::NoIcon,
                            "提示",
                            "文件已经存在，是否替换?",
                            QMessageBox::Yes | QMessageBox::No,
                            NULL);
        if (message.exec() == QMessageBox::No)
            return;
    }

    //进度条
    QProgressDialog *progress;
    progress = new QProgressDialog(this);
    ;
    progress->setCancelButton(nullptr);
    progress->setModal(true);
    progress->setFixedSize(QSize(300, 50));
    progress->setWindowTitle("进度");
    progress->setLabelText("正在压缩......");
    progress->show();
    progress->setValue(0);
    progress->setStyleSheet(
        "QProgressBar{border: 1px solid grey;border-radius: 5px;text-align: center;}"
        "QProgressBar::chunk{background-color: #CD96CD;width: 10px;margin: 0.5px;}");
    //转char*
    std::string ta = chose.toStdString();
    std::string tb = aim.toStdString();
    const char *srcPath = ta.c_str(); //源文件
    const char *dstPath = tb.c_str(); //目标文件路径
    std::map<int,std::string> code;
    LLint w[256]={0};
    //memset(w,0,sizeof(w));
    try {
        clock_t startTime, endTime; //统计时间
        startTime = clock();
        ZIP::encode(srcPath, dstPath, progress,code,w); //调用压缩方法
        endTime = clock();
        double time = double(endTime - startTime) / CLOCKS_PER_SEC;
        QFile filein(srcPath);
        QFile fileout(dstPath);
        qint64 fileinSize = filein.size();
        qint64 fileoutSize = fileout.size();
        filein.close();
        fileout.close();

        // 计算文件压缩比例
        double compressionRatio = (fileinSize == 0) ? 0.0 : (double(fileoutSize) / fileinSize) * 100;

        // 创建显示的信息字符串
        QString infoText = QString("压缩成功！\n"
                                   "原始文件大小: %1 bytes\n"
                                   "压缩后文件大小: %2 bytes\n"
                                   "压缩比例: %3%\n"
                                   "耗时: %4 秒")
                               .arg(fileinSize)              // 显示原始文件大小
                               .arg(fileoutSize)             // 显示压缩后文件大小
                               .arg(compressionRatio, 0, 'f', 2)  // 显示压缩比例，保留两位小数
                               .arg(time);                   // 显示耗时

        // 弹出消息框显示结果
        QMessageBox::information(nullptr, "提示", infoText);
        //QMessageBox::information(this, "提示", QString("压缩成功！耗时：%1s").arg(time));
        ui->stackedWidget->setCurrentIndex(4);
        ui->codetable->setRowCount(256);
        for(int i =0;i<256;i++)
        {
            qDebug()<<w[i]<<" ";
        }
        for(int row=0;row<256;row++)
        {
            QChar c(row);
            QString temp(c);

            QTableWidgetItem* charItem = new QTableWidgetItem(temp);
            QTableWidgetItem* freItem = new QTableWidgetItem(QString::number(w[row]));
            QTableWidgetItem* codeItem;
            if(w[row]==0)
            {
                codeItem = new QTableWidgetItem(QString("None"));
            }
            else
            {
                codeItem = new QTableWidgetItem(QString::fromStdString(code[row]));
            }
            QFont font("Arial Unicode MS");
            charItem->setFont(font);
            ui->codetable->setItem(row,0,charItem);
            ui->codetable->setItem(row,1,freItem);
            ui->codetable->setItem(row,2,codeItem);
        }
    } catch (std::runtime_error er) {
        qDebug() << er.what();
        QMessageBox::warning(this, "压缩失败", er.what());
    }
    progress->close();
}

//主页 压缩按钮
void MainWindow::on_btnCmpress_clicked()
{
    //转到压缩页
    ui->stackedWidget->setCurrentIndex(1);

    // 启用拖放功能
    setAcceptDrops(true);
}

// 拖动进入事件处理函数
void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasUrls()) {
        event->acceptProposedAction();  // 接受拖动操作
    }
}

// 拖动释放事件处理函数
void MainWindow::dropEvent(QDropEvent *event)
{
    QList<QUrl> urlList = event->mimeData()->urls();
    if (!urlList.isEmpty()) {
        QString fileName = urlList.first().toLocalFile();  // 获取文件路径
        qDebug() << "文件路径：" << fileName;

        if(ui->stackedWidget->currentIndex() == 1)//判断当前的界面
        {
            // 更新标签显示的文本
            ui->leComChose->setText(fileName);
            int flag = 0;
            for (int i = fileName.length()-1; i > 0; i--)
                if (fileName[i] == '.') {
                    flag = i;
                    break;
                }
            fileName.remove(flag, fileName.length() - flag);
            fileName.append(".hzip");
            ui->leComAim->setText(fileName);
        }
        else if(ui->stackedWidget->currentIndex() == 2)
        {
            // 判断文件是否以 ".hzip" 结尾（大小写不敏感）
            if (fileName.endsWith(".hzip", Qt::CaseInsensitive)) {
                ui->leUnChose->setText(fileName);
                QFileInfo fileInfo(fileName);  // 创建 QFileInfo 对象
                QString folderPath = fileInfo.absolutePath();  // 获取文件所在的文件夹路径
                ui->leUnAim->setText(folderPath);
                std::string tPath = fileName.toStdString();
                const char *path = tPath.c_str();
                try {
                    ui->leUnname->setText(QString(ZIP::getZipFileName(path)));
                } catch (std::runtime_error er) {
                    qDebug() << er.what();
                    QMessageBox::warning(this, "错误", er.what());
                }
            } else {
                QMessageBox::warning(this, "错误", "文件类型错误");
            }
        }
        else if(ui->stackedWidget->currentIndex() == 5)
        {
            if (fileName.endsWith(".hzip", Qt::CaseInsensitive))
            {
                ui->pathlineEdit->setText(fileName);
            }
            else
            {
                QMessageBox::warning(this, "错误", "文件类型错误");
            }
        }
        else if(ui->stackedWidget->currentIndex() == 6)
        {
            // 判断拖入的是否是文件夹
            QDir dir(fileName);
            if (dir.exists()) {
                ui->lineEdit->setText(fileName);
                qDebug() << "文件夹名称：" << fileName;
            } else {
                if (fileName.endsWith(".hzip", Qt::CaseInsensitive)) {
                    ui->lineEdit_2->setText(fileName);
                }
                else
                {
                    QMessageBox::warning(this, "错误", "文件类型错误");
                }
            }
        }
        else if(ui->stackedWidget->currentIndex() == 7)
        {
            // 判断拖入的是否是文件夹
            QDir dir(fileName);
            if (dir.exists()) {
                ui->lineEdit_4->setText(fileName);
                qDebug() << "文件夹名称：" << fileName;
            } else {
                if (fileName.endsWith(".hzip", Qt::CaseInsensitive)) {
                    ui->lineEdit_3->setText(fileName);
                }
                else
                {
                    QMessageBox::warning(this, "错误", "文件类型错误");
                }
            }
        }
    }
}

//主页 解压按钮
void MainWindow::on_btnUncompress_clicked()
{
    //转到解压页
    ui->stackedWidget->setCurrentIndex(2);
    // 启用拖放功能
    setAcceptDrops(true);
}
//主页 历史记录按钮
void MainWindow::on_btnHistory_clicked()
{
    //转到历史记录页
    ui->stackedWidget->setCurrentIndex(3);
    //加载显示历史记录

    QStringListModel *model;
    model = new QStringListModel(this);

    QFile hisfile("F:/QTProject/HuffmanZip/history.txt");

    QStringList his;
    if (hisfile.open(QIODevice::ReadOnly | QIODevice::Text)) {  // 以只读文本方式打开文件
        // 文件操作代码
        while (!hisfile.atEnd()) {
            QByteArray line = hisfile.readLine();  // 逐行读取文件内容
            if(line.trimmed()!="") his<<line.trimmed();//去空
            //qDebug() << line;  // 输出每行内容
        }

    } else {
        QMessageBox::critical(this,"error","file is not exeits");
    }
    hisfile.close();
    // 去除重复项
    his.removeDuplicates();  // 去除重复项
    model->setStringList(his);
    ui->hislistView->setModel(model);
    // 禁止用户编辑列表项
    ui->hislistView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    QObject::connect(ui->hislistView, &QListView::clicked, [this,model](const QModelIndex &index) {
        // 获取点击项的文本
        QString filename = model->data(index, Qt::DisplayRole).toString();

        // 显示消息框，提示点击了哪个项
        QMessageBox::information(nullptr, "Item Clicked", "You clicked on: " + filename);

        ui->stackedWidget->setCurrentIndex(2);
        ui->leUnChose->setText(filename);

        ui->leUnAim->setText(getDirectoryFromPath(filename));
        std::string tPath = filename.toStdString();
        const char *path = tPath.c_str();
        try {
            ui->leUnname->setText(QString(ZIP::getZipFileName(path)));
        } catch (std::runtime_error er) {
            qDebug() << er.what();
            QMessageBox::warning(this, "错误", er.what());
        }

    });
}

//解压页 取消按钮
void MainWindow::on_btnUnCancel_clicked()
{
    //返回主页
    ui->stackedWidget->setCurrentIndex(0);
}

//压缩页 取消按钮
void MainWindow::on_btnComCancel_clicked()
{
    //返回主页
    ui->stackedWidget->setCurrentIndex(0);
}

//压缩页 选择文件按钮
void MainWindow::on_btnComChose_clicked()
{
    //定义文件对话类框
    QFileDialog *fileDialog = new QFileDialog(this);
    //定义文件对话框标题
    fileDialog->setWindowTitle("Please Select one file");
    fileDialog->directory();
    //设置视图模式
    fileDialog->setViewMode(QFileDialog::Detail);
    //文本框显示选择的文件路径
    if (fileDialog->exec() == QFileDialog::Accepted) {
        QString fileName = fileDialog->selectedFiles().first();
        ui->leComChose->setText(fileName);
        int flag = 0;
        for (int i = fileName.length()-1; i > 0; i--)
            if (fileName[i] == '.') {
                flag = i;
                break;
            }
        fileName.remove(flag, fileName.length() - flag);
        fileName.append(".hzip");
        ui->leComAim->setText(fileName);
    }
}

//压缩页 压缩到按钮：选择压缩文件放置路径
void MainWindow::on_btnComAim_clicked()
{
    //定义文件对话类框
    QFileDialog *fileDialog = new QFileDialog(this);
    //定义文件对话框标题
    fileDialog->setWindowTitle("Please Select one file");
    //设置视图模式
    fileDialog->setViewMode(QFileDialog::Detail);
    fileDialog->setNameFilter("压缩文件(*.hzip)");
    //文本框显示选择的文件路径
    QString filenames;
    if (fileDialog->exec() == QFileDialog::Accepted) {
        filenames = fileDialog->selectedFiles().first();
        if (!filenames.endsWith(".hzip")) {
            filenames.append(".hzip");
        }
        ui->leComAim->setText(filenames);
    }
}

//解压页 选择文件按钮
void MainWindow::on_btnUnChose_clicked()
{
    //定义文件对话类框
    QFileDialog *fileDialog = new QFileDialog(this);
    //定义文件对话框标题
    fileDialog->setWindowTitle("Please Select one zip file");
    //设置文件过滤器
    fileDialog->setNameFilter("压缩文件(*.hzip)");
    //设置视图模式
    fileDialog->setViewMode(QFileDialog::Detail);
    //文本框显示选择的文件路径
    QString filename;
    if (fileDialog->exec() == QDialog::Accepted) {
        filename = fileDialog->selectedFiles().first();
        ui->leUnChose->setText(filename);
        ui->leUnAim->setText(fileDialog->directory().absolutePath());
        std::string tPath = filename.toStdString();
        const char *path = tPath.c_str();
        try {
            ui->leUnname->setText(QString(ZIP::getZipFileName(path)));
        } catch (std::runtime_error er) {
            qDebug() << er.what();
            QMessageBox::warning(this, "错误", er.what());
        }
    }
}

//解压页 解压到按钮
void MainWindow::on_btnUnAim_clicked()
{
    //定义文件对话类框
    QFileDialog *fileDialog = new QFileDialog(this);
    //设置视图模式
    fileDialog->setViewMode(QFileDialog::Detail);
    //文本框显示选择的文件路径
    QString filePath = fileDialog->getExistingDirectory(this, "Please Select Directory", "C:");
    if (!filePath.isEmpty())
        ui->leUnAim->setText(filePath);
}

//压缩页 确定按钮
void MainWindow::on_btnComsure_clicked()
{
    QString aim = ui->leComAim->text();
    QString chose = ui->leComChose->text();

    if (aim.isEmpty() || chose.isEmpty()) {
        QMessageBox::warning(this, "压缩失败", "请完整填写参数！");
        return;
    }

    //判断文件情况
    QFileInfo unzip(chose), zip(aim);
    if (!unzip.isFile()) {
        QMessageBox::warning(this, "压缩失败", "选择文件不存在！");
        return;
    } else if (zip.isFile()) {
        QMessageBox message(QMessageBox::NoIcon,
                            "提示",
                            "文件已经存在，是否替换?",
                            QMessageBox::Yes | QMessageBox::No,
                            NULL);
        if (message.exec() == QMessageBox::No)
            return;
    }

    //进度条
    QProgressDialog *progress;
    progress = new QProgressDialog(this);
    ;
    progress->setCancelButton(nullptr);
    progress->setModal(true);
    progress->setFixedSize(QSize(300, 50));
    progress->setWindowTitle("进度");
    progress->setLabelText("正在压缩......");
    progress->show();
    progress->setValue(0);
    progress->setStyleSheet(
        "QProgressBar{border: 1px solid grey;border-radius: 5px;text-align: center;}"
        "QProgressBar::chunk{background-color: #CD96CD;width: 10px;margin: 0.5px;}");

    std::string ta = chose.toStdString();
    std::string tb = aim.toStdString();
    const char *srcPath = ta.c_str(); //源文件
    const char *dstPath = tb.c_str(); //目标文件路径
    std::map<int,std::string> code;
    LLint w[256]={0};
    //memset(w,0,sizeof(w));

    try {
        clock_t startTime, endTime; //统计时间
        startTime = clock();
        ZIP::encode(srcPath, dstPath, progress,code,w); //调用压缩方法
        endTime = clock();
        double time = double(endTime - startTime) / CLOCKS_PER_SEC;
        QFile filein(srcPath);
        QFile fileout(dstPath);
        qint64 fileinSize = filein.size();
        qint64 fileoutSize = fileout.size();
        filein.close();
        fileout.close();

        // 计算文件压缩比例（可选）
        double compressionRatio = (fileinSize == 0) ? 0.0 : (double(fileoutSize) / fileinSize) * 100;

        // 创建显示的信息字符串
        QString infoText = QString("压缩成功！\n"
                                   "原始文件大小: %1 bytes\n"
                                   "压缩后文件大小: %2 bytes\n"
                                   "压缩比例: %3%\n"
                                   "耗时: %4 秒")
                               .arg(fileinSize)              // 显示原始文件大小
                               .arg(fileoutSize)             // 显示压缩后文件大小
                               .arg(compressionRatio, 0, 'f', 2)  // 显示压缩比例，保留两位小数
                               .arg(time);                   // 显示耗时

        // 弹出消息框显示结果
        QMessageBox::information(nullptr, "提示", infoText);
        //QMessageBox::information(this, "提示", QString("压缩成功！耗时：%1s").arg(time));
        ui->stackedWidget->setCurrentIndex(4);
        ui->codetable->setRowCount(256);

    } catch (std::runtime_error er) {
        qDebug() << er.what();
        QMessageBox::warning(this, "压缩失败", er.what());
    }
    progress->close();
    for(int row=0;row<256;row++)
    {
        QChar c(row);
        QString temp(c);

        QTableWidgetItem* charItem = new QTableWidgetItem(temp);
        QTableWidgetItem* freItem = new QTableWidgetItem(QString::number(w[row]));
        QTableWidgetItem* codeItem;
        if(w[row]==0)
        {
            codeItem = new QTableWidgetItem(QString("None"));
        }
        else
        {
            codeItem = new QTableWidgetItem(QString::fromStdString(code[row]));
        }
        QFont font("Arial Unicode MS");
        charItem->setFont(font);
        ui->codetable->setItem(row,0,charItem);
        ui->codetable->setItem(row,1,freItem);
        ui->codetable->setItem(row,2,codeItem);
    }

}

//解压页 确定按钮
void MainWindow::on_btnUnSure_clicked()
{
    //获取文本
    QString aim = ui->leUnAim->text();     //解压到文件夹
    QString chose = ui->leUnChose->text(); //压缩文件路径
    QString name = ui->leUnname->text();   //解压后的文件名
    QString path;                          //解压后的路径

    if (aim.isEmpty() || chose.isEmpty() || name.isEmpty()) {
        QMessageBox::warning(this, "解压失败", "请完整填写参数！");
        return;
    }


    //添加路径分割符'/'
    if (aim.endsWith("/") || aim.endsWith("\\"))
        path = aim.append(name);
    else
        path = aim.append("/").append(name);

    //判断文件情况
    QFileInfo zip(chose), unzip(path);
    if (!zip.isFile()) {
        QMessageBox::warning(this, "解压失败", "压缩文件不存在！");
        return;
    } else if (unzip.isFile()) {
        QMessageBox message(QMessageBox::NoIcon,
                            "提示",
                            "文件已经存在，是否替换?",
                            QMessageBox::Yes | QMessageBox::No,
                            NULL);
        if (message.exec() == QMessageBox::No)
            return;
    }

    std::string ta = chose.toStdString();
    std::string tb = path.toStdString();
    const char *zipPath = ta.c_str(); //压缩文件路径
    const char *dstPath = tb.c_str();
    ; //目标文件夹

    //进度条
    QProgressDialog *progress;
    progress = new QProgressDialog(this);
    ;
    progress->setCancelButton(nullptr);
    progress->setModal(true);
    progress->setFixedSize(QSize(300, 50));
    progress->setWindowTitle("进度");
    progress->setLabelText("正在解压......");
    progress->show();
    progress->setValue(0);
    progress->setStyleSheet(
        "QProgressBar{border: 1px solid grey;border-radius: 5px;text-align: center;}"
        "QProgressBar::chunk{background-color: #CD96CD;width: 10px;margin: 0.5px;}");

    try {
        clock_t startTime, endTime; //统计时间
        startTime = clock();
        ZIP::decode(zipPath, dstPath, progress); //调用解压方法
        endTime = clock();
        double time = double(endTime - startTime) / CLOCKS_PER_SEC;
        qDebug() << time;
        QMessageBox::information(this, "提示", QString("解压成功！耗时：%1s").arg(time));
    } catch (std::runtime_error er) {
        qDebug() << er.what();
        QMessageBox::warning(this, "解压失败", er.what());
    }
    progress->close();
}

// 历史记录页 返回主页
void MainWindow::on_pushButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

// 编码对照表 返回主页
void MainWindow::on_fan_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

// 文本压缩 进入文本压缩页面
void MainWindow::on_textzip_clicked()
{
    ui->stackedWidget->setCurrentIndex(5);
    // 启用拖放功能
    setAcceptDrops(true);
}

// 文本压缩页 压缩到
void MainWindow::on_zippathButton_clicked()
{
    //定义文件对话类框
    QFileDialog *fileDialog = new QFileDialog(this);
    //定义文件对话框标题
    fileDialog->setWindowTitle("Please Select one file");
    //设置视图模式
    fileDialog->setViewMode(QFileDialog::Detail);
    fileDialog->setNameFilter("压缩文件(*.hzip)");
    //文本框显示选择的文件路径
    QString filenames;
    if (fileDialog->exec() == QFileDialog::Accepted) {
        filenames = fileDialog->selectedFiles().first();
        if (!filenames.endsWith(".hzip")) {
            filenames.append(".hzip");
        }
        ui->pathlineEdit->setText(filenames);
    }

}

void MainWindow::on_zipexitButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

// 文本压缩 确定按钮
void MainWindow::on_zipButton_clicked()
{
    // 获取 QTextEdit 的内容
    QString text = ui->textEdit->toPlainText();
    QString zipfilePath = ui->pathlineEdit->text();

    if(text.isEmpty()||zipfilePath.isEmpty())
    {
        QMessageBox::warning(this, "压缩失败", "请完整填写参数！");
        return;
    }

    QString srcfilePath = zipfilePath;
    //qDebug()<<text;
    qDebug()<<zipfilePath;

    int flag = 0;
    for (int i = zipfilePath.length()-1; i > 0; i--)
        if (zipfilePath[i] == '.') {
            flag = i;
            break;
        }
    srcfilePath.remove(flag, srcfilePath.length() - flag);
    srcfilePath.append(".doc");
    qDebug()<<srcfilePath;
    QFile file(srcfilePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out.setEncoding(QStringConverter::Encoding::Utf8); // 设置编码为 UTF-8
        out << text; // 写入内容
        file.close();
        // QMessageBox::information(this, "成功", "文件保存成功！");
    } else {

        QMessageBox::warning(this, "错误", "无法保存文件！");
    }

    ziphan(zipfilePath,srcfilePath);
}


// 文件夹压缩 选择文件
void MainWindow::on_pushButton_2_clicked()
{
    // 定义文件对话框
    QFileDialog *fileDialog = new QFileDialog(this);
    // 设置视图模式为详细模式，这样可以显示文件和文件夹的详细信息
    fileDialog->setViewMode(QFileDialog::Detail);
    // 弹出对话框让用户选择一个目录，返回选择的目录路径
    QString filePath = fileDialog->getExistingDirectory(this, "Please Select Directory", "C:");
    // 如果用户选择了一个目录（即filePath不为空），则将路径显示在文本框中
    if (!filePath.isEmpty())
    {
        ui->lineEdit->setText(filePath);
    }
}

// 文件夹压缩页
void MainWindow::on_btndirencode_clicked()
{
    ui->stackedWidget->setCurrentIndex(6);
    // 启用拖放功能
    setAcceptDrops(true);
}

// 文件夹压缩页 压缩到
void MainWindow::on_pushButton_3_clicked()
{
    //定义文件对话类框
    QFileDialog *fileDialog = new QFileDialog(this);
    //定义文件对话框标题
    fileDialog->setWindowTitle("Please Select one file");
    //设置视图模式
    fileDialog->setViewMode(QFileDialog::Detail);
    fileDialog->setNameFilter("压缩文件(*.hzip)");
    //文本框显示选择的文件路径
    QString filenames;
    if (fileDialog->exec() == QFileDialog::Accepted) {
        filenames = fileDialog->selectedFiles().first();
        if (!filenames.endsWith(".hzip")) {
            filenames.append(".hzip");
        }
        ui->lineEdit_2->setText(filenames);
    }
}

// 文件夹压缩 取消键
void MainWindow::on_btndirecexit_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

// 文件夹压缩，确认键
void MainWindow::on_btndirec_clicked()
{
    QString aim = ui->lineEdit->text();
    QString chose = ui->lineEdit_2->text();
    qDebug()<<aim;
    if (aim.isEmpty() || chose.isEmpty()) {
        QMessageBox::warning(this, "压缩失败", "请完整填写参数！");
        return;
    }

    //判断文件情况
    QFileInfo unzip(chose), zip(aim);
    if (!unzip.isFile()) {
        QMessageBox::warning(this, "压缩失败", "选择文件不存在！");
        return;
    } else if (zip.isFile()) {
        QMessageBox message(QMessageBox::NoIcon,
                            "提示",
                            "文件已经存在，是否替换?",
                            QMessageBox::Yes | QMessageBox::No,
                            NULL);
        if (message.exec() == QMessageBox::No)
            return;
    }

    //进度条
    QProgressDialog *progress;
    progress = new QProgressDialog(this);
    ;
    progress->setCancelButton(nullptr);
    progress->setModal(true);
    progress->setFixedSize(QSize(100, 50));
    progress->setWindowTitle("进度");
    progress->setLabelText("正在压缩......");
    progress->show();
    progress->setValue(0);
    progress->setStyleSheet(
        "QProgressBar{border: 1px solid grey;border-radius: 5px;text-align: center;}"
        "QProgressBar::chunk{background-color: #CD96CD;width: 10px;margin: 0.5px;}");

    std::string ta = aim.toStdString();
    std::string tb = chose.toStdString();
    const char *srcPath = ta.c_str(); //源文件
    const char *dstPath = tb.c_str(); //目标文件路径
    clock_t startTime, endTime; //统计时间
    startTime = clock();
    LLint w[256]={0};
    std::map<int,std::string> code;
    ZIP::direncode(srcPath,dstPath,progress,code,w);

    endTime=clock();

    QFile fileout(dstPath);
    qint64 fileinSize = ZIP::getFolderSize(srcPath);
    qint64 fileoutSize = fileout.size();

    fileout.close();

    double time = double(endTime - startTime) / CLOCKS_PER_SEC;
    // 计算文件压缩比例（可选）
    double compressionRatio = (fileinSize == 0) ? 0.0 : (double(fileoutSize) / fileinSize) * 100;

    // 创建显示的信息字符串
    QString infoText = QString("压缩成功！\n"
                               "原始文件大小: %1 bytes\n"
                               "压缩后文件大小: %2 bytes\n"
                               "压缩比例: %3%\n"
                               "耗时: %4 秒")
                           .arg(fileinSize)              // 显示原始文件大小
                           .arg(fileoutSize)             // 显示压缩后文件大小
                           .arg(compressionRatio, 0, 'f', 2)  // 显示压缩比例，保留两位小数
                           .arg(time);                   // 显示耗时
    // 弹出消息框显示结果
    QMessageBox::information(nullptr, "提示", infoText);

    ui->stackedWidget->setCurrentIndex(4);
    ui->codetable->setRowCount(256);
    for(int row=0;row<256;row++)
    {
        QChar c(row);
        QString temp(c);

        QTableWidgetItem* charItem = new QTableWidgetItem(temp);
        QTableWidgetItem* freItem = new QTableWidgetItem(QString::number(w[row]));
        QTableWidgetItem* codeItem;
        if(w[row]==0)
        {
            codeItem = new QTableWidgetItem(QString("None"));
        }
        else
        {
            codeItem = new QTableWidgetItem(QString::fromStdString(code[row]));
        }
        QFont font("Arial Unicode MS");
        charItem->setFont(font);
        ui->codetable->setItem(row,0,charItem);
        ui->codetable->setItem(row,1,freItem);
        ui->codetable->setItem(row,2,codeItem);
    }
    progress->close();
}

// 文件夹解压页
void MainWindow::on_btndirdecode_clicked()
{
    ui->stackedWidget->setCurrentIndex(7);
    // 启用拖放功能
    setAcceptDrops(true);
}

// 文件夹解压页 选择文件
void MainWindow::on_pushButton_4_clicked()
{
    //定义文件对话类框
    QFileDialog *fileDialog = new QFileDialog(this);
    //定义文件对话框标题
    fileDialog->setWindowTitle("Please Select one file");
    //设置视图模式
    fileDialog->setViewMode(QFileDialog::Detail);
    fileDialog->setNameFilter("压缩文件(*.hzip)");
    //文本框显示选择的文件路径
    QString filenames;
    if (fileDialog->exec() == QFileDialog::Accepted) {
        filenames = fileDialog->selectedFiles().first();
        if (!filenames.endsWith(".hzip")) {
            filenames.append(".hzip");
        }
        ui->lineEdit_3->setText(filenames);
    }
}

// 文件夹解压页 解压到
void MainWindow::on_pushButton_5_clicked()
{
    // 定义文件对话框
    QFileDialog *fileDialog = new QFileDialog(this);
    // 设置视图模式为详细模式，这样可以显示文件和文件夹的详细信息
    fileDialog->setViewMode(QFileDialog::Detail);
    // 弹出对话框让用户选择一个目录，返回选择的目录路径
    QString filePath = fileDialog->getExistingDirectory(this, "Please Select Directory", "C:");
    // 如果用户选择了一个目录（即filePath不为空），则将路径显示在文本框中
    if (!filePath.isEmpty())
    {
        ui->lineEdit_4->setText(filePath);
    }
}
//文件夹解压页 取消键
void MainWindow::on_dirdcexit_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

// 文件夹页解压 确定
void MainWindow::on_btndirdc_clicked()
{
    //获取文本
    QString aim = ui->lineEdit_4->text();     //解压到文件夹
    QString chose = ui->lineEdit_3->text(); //压缩文件路径
    if (aim.isEmpty() || chose.isEmpty()) {
        QMessageBox::warning(this, "解压失败", "请完整填写参数！");
        return;
    }
    //判断文件情况
    QFileInfo unzip(chose), zip(aim);
    if (!unzip.isFile()) {
        QMessageBox::warning(this, "解压失败", "选择文件不存在！");
        return;
    }
    std::string ta = chose.toStdString();
    std::string tb = aim.toStdString();
    const char *zipPath = ta.c_str(); //压缩文件路径
    const char *dstPath = tb.c_str();
    ;
    // 目标文件夹
    // 进度条
    QProgressDialog *progress;
    progress = new QProgressDialog(this);
    ;
    progress->setCancelButton(nullptr);
    progress->setModal(true);
    progress->setFixedSize(QSize(300, 50));
    progress->setWindowTitle("进度");
    progress->setLabelText("正在解压......");
    progress->show();
    progress->setValue(0);
    progress->setStyleSheet(
        "QProgressBar{border: 1px solid grey;border-radius: 5px;text-align: center;}"
        "QProgressBar::chunk{background-color: #CD96CD;width: 10px;margin: 0.5px;}");

    try {
        clock_t startTime, endTime; //统计时间
        startTime = clock();
        ZIP::dirdecode(zipPath,dstPath,progress);
        endTime=clock();
        double time = double(endTime - startTime) / CLOCKS_PER_SEC;
        QString infoText = QString("解压成功！\n"
                                   "耗时: %4 秒")
                                    .arg(time);                   // 显示耗时

        // 弹出消息框显示结果
        QMessageBox::information(nullptr, "提示", infoText);
    } catch (std::runtime_error er) {
        qDebug() << er.what();
        QMessageBox::warning(this, "解压失败", er.what());
    }
    progress->close();
}
