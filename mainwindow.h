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

private slots:
    void openSerialPort();
    void closeSerialPort();
    void about();
    void writeData(const QByteArray &data);
    void readData();

    void handleError(QSerialPort::SerialPortError error);

private:
    void initActionsConnections();

private:
    void showStatusMessage(const QString &message);

    Ui::MainWindow *ui;
    QLabel *status;
    //Console *console;
    settingsdialog *settings;
    QSerialPort *serial;
};

#endif // MAINWINDOW_H
