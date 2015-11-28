#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtCore/QtGlobal>
#include "settingsdialog.h"
#include <QMainWindow>
#include <QMessageBox.h>
#include <QtSerialPort/QSerialPort>

QT_BEGIN_NAMESPACE

class QLabel;


namespace Ui {
class MainWindow;
}

QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    struct receivedDataFrame
    {
        qint8  id;
        qint16 data;
        qint16 checksum;
    };

private slots:
    void about();
    void openSerialPort();
    void closeSerialPort();

    void writeData(const QByteArray &data);
    void readData();
    void showCurrentSetting();
    void handleError(QSerialPort::SerialPortError error);
private Q_SLOTS:
     void onReadyRead();
     void on_sendPushButton_clicked();

     void on_clearPushButton_clicked();

private:
    void initActionsConnections();

private:
    void showStatusMessage(const QString &message);

    Ui::MainWindow *ui;
    QLabel *status;
    receivedDataFrame *rFrame;

    //Console *console;
    settingsdialog *settings;
    QSerialPort *serial;
    short flag =0;
};

#endif // MAINWINDOW_H
