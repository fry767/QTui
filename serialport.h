#ifndef SERIALPORT_H
#define SERIALPORT_H
#include <iostream>
#include <QtSerialPort/QSerialPort>

class SerialPort
{
public:
    SerialPort();
    ~SerialPort();

private Q_SLOTS:
    void onReadyRead();

};

#endif // SERIALPORT_H
