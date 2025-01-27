#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QBrush>
#include <QColor>
#include <QDebug>
#include <QFileDialog>
#include <QGraphicsDropShadowEffect>
#include <QMainWindow>
#include <QPalette>
#include <QPixmap>
#include <QString>
#include <QStringList>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_btnCmpress_clicked();

    void on_btnUncompress_clicked();

    void on_btnUnCancel_clicked();

    void on_btnComCancel_clicked();

    void on_btnComChose_clicked();

    void on_btnComAim_clicked();

    void on_btnUnChose_clicked();

    void on_btnUnAim_clicked();

    void on_btnComsure_clicked();

    void on_btnUnSure_clicked();

    void on_btnHistory_clicked();

    void on_pushButton_clicked();

    void on_fan_clicked();

    void on_textzip_clicked();

    QString getDirectoryFromPath(const QString &filePath);


    void on_zippathButton_clicked();

    void ziphan(QString aim,QString chose);

    void on_zipexitButton_clicked();

    void on_zipButton_clicked();

    void on_pushButton_2_clicked();

    void on_btndirencode_clicked();

    void on_pushButton_3_clicked();

    void on_btndirecexit_clicked();

    void on_btndirec_clicked();

    void on_btndirdecode_clicked();

    void on_pushButton_4_clicked();

    void on_dirdcexit_clicked();

    void on_pushButton_5_clicked();

    void on_btndirdc_clicked();
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
