#include "configserialport.h"
#include "ui_configserialport.h"

ConfigSerialPort::ConfigSerialPort(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfigSerialPort)
{
    ui->setupUi(this);
}

ConfigSerialPort::~ConfigSerialPort()
{
    delete ui;
}
