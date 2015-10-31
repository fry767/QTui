#include "settingsdialog.h"
#include "ui_settingsdialog.h"

#include <QtSerialPort/QSerialPortInfo>
#include <QIntValidator>
#include <QLineEdit>

QT_USE_NAMESPACE
static const char blankString[] = QT_TRANSLATE_NOOP("SettingsDialog", "N/A");

settingsdialog::settingsdialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::settingsdialog)
{
    ui->setupUi(this);
    intValidator = new QIntValidator(0,4000000,this);
    ui->baudRateComboBox->setInsertPolicy(QComboBox::NoInsert);

    connect(ui->applyPushButton,SIGNAL(clicked()),this,
            SLOT(apply()));
    connect(ui->comPortComboBox,SIGNAL(currentIndexChanged(int)),this,
            SLOT(showPortInfo(int)));
    connect(ui->baudRateComboBox,SIGNAL(currentIndexChanged(int)),this,
            SLOT(checkCustomBaudRatePolicy(int)));
    connect(ui->comPortComboBox,SIGNAL(currentIndexChanged(int)),this,
            SLOT(checkCustomDevicePathPolicy(int)));
    fillPortsParameters();
    fillPortsInfo();
    updateSettings();
}

settingsdialog::~settingsdialog()
{
    delete ui;
}
settingsdialog::Settings settingsdialog::settings() const
{
    return currentSettings;
}
void settingsdialog :: showPortInfo(int idx)
{
    if(idx == -1)
        return;
    QStringList list = ui->comPortComboBox->itemData(idx).toStringList();

    ui->descriptionLabel->setText(tr("Description: %1").arg(list.count() > 1 ? list.at(1) : tr(blankString)));
    ui->manufacturerLabel->setText(tr("Manufacturer: %1").arg(list.count() > 2 ? list.at(2) : tr(blankString)));
    ui->serialNumberLabel->setText(tr("Serial number: %1").arg(list.count() > 3 ? list.at(3) : tr(blankString)));
    ui->locationLabel->setText(tr("Location: %1").arg(list.count() > 4 ? list.at(4) : tr(blankString)));
    ui->vendorIdLabel->setText(tr("Vendor Identifier: %1").arg(list.count() > 5 ? list.at(5) : tr(blankString)));
    ui->productIdLabel->setText(tr("Product Identifier: %1").arg(list.count() > 6 ? list.at(6) : tr(blankString)));
}
void settingsdialog::apply()
{
    updateSettings();
    hide();
}
void settingsdialog :: checkCustomBaudRatePolicy(int idx)
{
    bool isCustomBaudRate = !ui->baudRateComboBox->itemData(idx).isValid();
    ui->baudRateComboBox->setEditable(isCustomBaudRate);
    if(isCustomBaudRate)
    {
        ui->baudRateComboBox->clearEditText();
        QLineEdit *edit = ui->baudRateComboBox->lineEdit();
        edit->setValidator(intValidator);
    }
}
void settingsdialog :: checkCustomDevicePathPolicy(int idx)
{
    bool isCustomPath = !ui->comPortComboBox->itemData(idx).isValid();
    ui->comPortComboBox->setEditable(isCustomPath);
    if(isCustomPath)
    {
        ui->comPortComboBox->clearEditText();
    }
}
void settingsdialog::fillPortsParameters()
{
    ui->baudRateComboBox->addItem(QStringLiteral("9600"),QSerialPort::Baud9600);
    ui->baudRateComboBox->addItem(QStringLiteral("19200"),QSerialPort::Baud19200);
    ui->baudRateComboBox->addItem(QStringLiteral("38400"),QSerialPort::Baud38400);
    ui->baudRateComboBox->addItem(QStringLiteral("57600"),QSerialPort::Baud57600);
    ui->baudRateComboBox->addItem(QStringLiteral("115200"),QSerialPort::Baud115200);

    ui->dataBitsComboBox->addItem(QStringLiteral("5"), QSerialPort::Data5);
    ui->dataBitsComboBox->addItem(QStringLiteral("6"), QSerialPort::Data6);
    ui->dataBitsComboBox->addItem(QStringLiteral("7"), QSerialPort::Data7);
    ui->dataBitsComboBox->addItem(QStringLiteral("8"), QSerialPort::Data8);
    ui->dataBitsComboBox->setCurrentIndex(3);

    ui->parityComboBox->addItem(tr("None"), QSerialPort::NoParity);
    ui->parityComboBox->addItem(tr("Even"), QSerialPort::EvenParity);
    ui->parityComboBox->addItem(tr("Odd"), QSerialPort::OddParity);
    ui->parityComboBox->addItem(tr("Mark"), QSerialPort::MarkParity);
    ui->parityComboBox->addItem(tr("Space"), QSerialPort::SpaceParity);

    ui->stopBitsComboBox->addItem(QStringLiteral("1"), QSerialPort::OneStop);
#ifdef Q_OS_WIN
    ui->stopBitsComboBox->addItem(tr("1.5"), QSerialPort::OneAndHalfStop);
#endif
    ui->stopBitsComboBox->addItem(QStringLiteral("2"), QSerialPort::TwoStop);

    ui->flowControlComboBox->addItem(tr("None"), QSerialPort::NoFlowControl);
    ui->flowControlComboBox->addItem(tr("RTS/CTS"), QSerialPort::HardwareControl);
    ui->flowControlComboBox->addItem(tr("XON/XOFF"), QSerialPort::SoftwareControl);
}
void settingsdialog::fillPortsInfo()
{
    ui->comPortComboBox->clear();
    QString description;
    QString manufacturer;
    QString serialNumber;
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        QStringList list;
        description = info.description();
        manufacturer = info.manufacturer();
        serialNumber = info.serialNumber();
        list << info.portName()
             << (!description.isEmpty() ? description : blankString)
             << (!manufacturer.isEmpty() ? manufacturer : blankString)
             << (!serialNumber.isEmpty() ? serialNumber : blankString)
             << info.systemLocation()
             << (info.vendorIdentifier() ? QString::number(info.vendorIdentifier(), 16) : blankString)
             << (info.productIdentifier() ? QString::number(info.productIdentifier(), 16) : blankString);

        ui->comPortComboBox->addItem(list.first(), list);
    }

    ui->comPortComboBox->addItem(tr("Custom"));
}
void settingsdialog::updateSettings()
{
    currentSettings.name = ui->comPortComboBox->currentText();

    if (ui->baudRateComboBox->currentIndex() == 4) {
        currentSettings.baudRate = ui->baudRateComboBox->currentText().toInt();
    } else {
        currentSettings.baudRate = static_cast<QSerialPort::BaudRate>(
                    ui->baudRateComboBox->itemData(ui->baudRateComboBox->currentIndex()).toInt());
    }
    currentSettings.stringBaudRate = QString::number(currentSettings.baudRate);

    currentSettings.dataBits = static_cast<QSerialPort::DataBits>(
                ui->dataBitsComboBox->itemData(ui->dataBitsComboBox->currentIndex()).toInt());
    currentSettings.stringDataBits = ui->dataBitsComboBox->currentText();

    currentSettings.parity = static_cast<QSerialPort::Parity>(
                ui->parityComboBox->itemData(ui->parityComboBox->currentIndex()).toInt());
    currentSettings.stringParity = ui->parityComboBox->currentText();

    currentSettings.stopBits = static_cast<QSerialPort::StopBits>(
                ui->stopBitsComboBox->itemData(ui->stopBitsComboBox->currentIndex()).toInt());
    currentSettings.stringStopBits = ui->stopBitsComboBox->currentText();

    currentSettings.flowControl = static_cast<QSerialPort::FlowControl>(
                ui->flowControlComboBox->itemData(ui->flowControlComboBox->currentIndex()).toInt());
    currentSettings.stringFlowControl = ui->flowControlComboBox->currentText();
    //local echo pas implémenter
    //currentSettings.localEchoEnabled = ui->localEchoCheckBox->isChecked();
}
