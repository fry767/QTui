#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    serial = new QSerialPort(this);
//! [0]
    settings = new settingsdialog;
    rFrame   = new receivedDataFrame;
    ui->connectPushButton->setEnabled(true);
    ui->disconnectPushButton->setEnabled(false);


    showCurrentSetting();
    initActionsConnections();


    connect(serial, SIGNAL(error(QSerialPort::SerialPortError)), this,
            SLOT(handleError(QSerialPort::SerialPortError)));

//! [2]
   connect(serial, SIGNAL(readyRead()), this, SLOT(onReadyRead()));

//! [2]
    //connect(console, SIGNAL(getData(QByteArray)), this, SLOT(writeData(QByteArray)));
//! [3]
}

MainWindow::~MainWindow()
{
    delete settings;
    delete ui;
}
void MainWindow::openSerialPort()
{

    settingsdialog::Settings p = settings->settings();
    serial->setPortName(p.name);
    serial->setBaudRate(p.baudRate);
    serial->setDataBits(p.dataBits);
    serial->setParity(p.parity);
    serial->setStopBits(p.stopBits);
    serial->setFlowControl(p.flowControl);

    if (serial->open(QIODevice::ReadWrite)) {
        //console->setEnabled(true);
       // console->setLocalEchoEnabled(p.localEchoEnabled);

        ui->connectPushButton->setEnabled(false);
        ui->disconnectPushButton->setEnabled(true);

        showStatusMessage(tr("Connected to %1 : %2, %3, %4, %5, %6")
                          .arg(p.name).arg(p.stringBaudRate).arg(p.stringDataBits)
                          .arg(p.stringParity).arg(p.stringStopBits).arg(p.stringFlowControl));
         showCurrentSetting();
    } else {

        QMessageBox::critical(this, tr("Error"), serial->errorString());

        showStatusMessage(tr("Open error"));
    }
}
void MainWindow::closeSerialPort()
{
    if (serial->isOpen())
        serial->close();
   // console->setEnabled(false);
    ui->connectPushButton->setEnabled(true);
    ui->disconnectPushButton->setEnabled(false);
    showStatusMessage(tr("Disconnected"));;
}
void MainWindow::about()
{
    QMessageBox::about(this, tr("About Simple Terminal"),
                       tr("The <b>Simple Terminal</b> example demonstrates how to "
                          "use the Qt Serial Port module in modern GUI applications "
                          "using Qt, with a menu bar, toolbars, and a status bar."));
}
void MainWindow::writeData(const QByteArray &data)
{
    serial->write(data);
}
void MainWindow::onReadyRead()
{
    static QByteArray data ;

    if(serial->bytesAvailable())
    {
        data.append(serial->readAll());

        int id =0;
        int size = data.size();
        if (data[0] == 0x5B && data[size-1]==0x5D)
        {

            QByteArray dataWithoutHeader;
            for(int i=1;i<(size-1);i++)
            {
                dataWithoutHeader.append(data[i]);
            }
            QList<QByteArray> list = dataWithoutHeader.split(0x2C);
            QString sRead(QString::fromLatin1(dataWithoutHeader));
            for(int i=0; i<list.size();i++)
            {
                id = list[i][0];
                QByteArray crap;
                for(int j=1;j<(list[i].size());j++)
                {
                    crap.append(list[i][j]);
                }
                QString string(QString::fromLatin1(crap));
                switch(id)
                {
                    case 1 :
                    ui->displayTextEdit->clear();
                    ui->displayTextEdit->insertPlainText(string);
                    break;
                    case 2 :
                    ui->display1TextEdit->clear();
                    ui->display1TextEdit->insertPlainText(string);
                    break;
                    case 3 :
                    ui->display2TextEdit->clear();
                    ui->display2TextEdit->insertPlainText(string);
                    break;
                    case 4 :
                    ui->display3TextEdit->clear();
                    ui->display3TextEdit->insertPlainText(string);
                    break;
                    case 5 :
                    ui->display4TextEdit->clear();
                    ui->display4TextEdit->insertPlainText(string);
                    break;
                    case 6 :
                    ui->display5TextEdit->clear();
                    ui->display5TextEdit->insertPlainText(string);
                    break;


                }
            }

            data.clear();
            QByteArray ack = "K";
            writeData(ack);

            //serial->clear(QSerialPort::AllDirections);



        }
    }

}
void MainWindow::readData()
{
   /* int numBytes = serial->bytesAvailable();
    char buff[256];
    serial->read(buff,numBytes);
   // QByteArray data = serial->readAll();
   // QString sData(data);
    ui->displayTextEdit->insertPlainText(QString::fromLatin1(buff));
    //console->putData(data);*/
}
void MainWindow::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
        QMessageBox::critical(this, tr("Critical Error"), serial->errorString());
        closeSerialPort();
    }
}
void MainWindow::initActionsConnections()
{

    connect(ui->connectPushButton,    SIGNAL(released()), this,    SLOT(openSerialPort()));
    connect(ui->disconnectPushButton, SIGNAL(released()), this,    SLOT(closeSerialPort()));
    connect(ui->configPushButton,     SIGNAL(released()), settings,SLOT(show()));
    connect(ui->actionQuitter,        SIGNAL(triggered()),this,    SLOT(close()));
    connect(ui->actionConfig,         SIGNAL(triggered()),settings,SLOT(show()));
  // connect(ui->clearPushButton, SIGNAL(triggered()), console, SLOT(clear()));
  //  connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(about()));
  //  connect(ui->actionAboutQt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}

void MainWindow::showStatusMessage(const QString &message)
{
    char* cMessage;
    cMessage = new char [message.size()+1];
    strcpy( cMessage, message.toStdString().c_str());
    QMessageBox::information(
           this,
           tr("Message"),
           tr(cMessage) );
   // status->setText(message);
}
void MainWindow::showCurrentSetting()
{
    settingsdialog::Settings currentSettings = settings->settings();
    ui->CurrentSettingLabel->setText(tr("Current setting are %1 : %2, %3, %4, %5, %6")
                                     .arg(currentSettings.name).arg(currentSettings.stringBaudRate).arg(currentSettings.stringDataBits)
                                     .arg(currentSettings.stringParity).arg(currentSettings.stringStopBits).arg(currentSettings.stringFlowControl));

}

void MainWindow::on_sendPushButton_clicked()
{
   QByteArray byteArray((const char*) (ui->sendLineEdit->text().utf16()), ui->sendLineEdit->text().size() * 2);
   writeData(byteArray);
}

void MainWindow::on_clearPushButton_clicked()
{
    serial->clear(QSerialPort::AllDirections);
    //serial->clearError();
    ui->displayTextEdit->clear();
    ui->display1TextEdit->clear();
    ui->display2TextEdit->clear();
    ui->display3TextEdit->clear();
    ui->display4TextEdit->clear();
    ui->display5TextEdit->clear();


}
