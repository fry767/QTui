#ifndef CONFIGSERIALPORT_H
#define CONFIGSERIALPORT_H

#include <QDialog>

namespace Ui {
class ConfigSerialPort;
}

class ConfigSerialPort : public QDialog
{
    Q_OBJECT

public:
    explicit ConfigSerialPort(QWidget *parent = 0);
    ~ConfigSerialPort();

private:
    Ui::ConfigSerialPort *ui;
};

#endif // CONFIGSERIALPORT_H
