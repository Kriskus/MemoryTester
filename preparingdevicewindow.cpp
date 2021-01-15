#include "preparingdevicewindow.h"
#include "ui_preparingdevicewindow.h"

PreparingDeviceWindow::PreparingDeviceWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PreparingDeviceWindow) {
    ui->setupUi(this);
    countCrc = new CounterCrc();
    confDevice = new DeviceConfiguration();

    connect(ui->pushButtonClose, &QPushButton::clicked, this, &PreparingDeviceWindow::hideWindow);
    connect(ui->pushButtonHeader, &QPushButton::clicked, this, &PreparingDeviceWindow::setHeader);
    connect(ui->pushButtonVatRates, &QPushButton::clicked, this, &PreparingDeviceWindow::setVatRates);
    connect(ui->pushButtonDailyReport, &QPushButton::clicked, this, &PreparingDeviceWindow::doDailyReport);

    sfskInfo.append("Tryb fiskalny      ");
    sfskInfo.append("Zerowania RAM      ");
    sfskInfo.append("Raporty dobowe     ");
    sfskInfo.append("Ilość zmian stawek ");
    sfskInfo.append("Stawka A           ");
    sfskInfo.append("Stawka B           ");
    sfskInfo.append("Stawka C           ");
    sfskInfo.append("Stawka D           ");
    sfskInfo.append("Stawka E           ");
    sfskInfo.append("Stawka F           ");
    sfskInfo.append("Stawka G           ");
    sfskInfo.append("Data ostatniego RD ");
    sfskInfo.append("Numer unikatowy    ");
}

PreparingDeviceWindow::~PreparingDeviceWindow() {
    delete ui;
    delete countCrc;
    delete confDevice;
}

void PreparingDeviceWindow::showWindow() {
    emit sendSequenceToDevice(countCrc->countCrc(confDevice->sfskStatus()));
}

void PreparingDeviceWindow::setDeviceInformation(QByteArray data) {
    ui->textBrowser->clear();
    int line = 0;
    QList<QByteArray> dataSplitted = data.split("\t");
    dataSplitted.removeFirst();
    dataSplitted.removeLast();
    foreach (data, dataSplitted) {
        ui->textBrowser->append(sfskInfo.at(line++) + "- " + data);
    }
}

void PreparingDeviceWindow::setHeader() {
    emit sendSequenceToDevice(countCrc->countCrc(confDevice->setHeader()));
    emit sendSequenceToDevice(countCrc->countCrc(confDevice->sfskStatus()));
}

void PreparingDeviceWindow::setVatRates() {
    emit sendSequenceToDevice(countCrc->countCrc(confDevice->setVatRates()));
    emit sendSequenceToDevice(countCrc->countCrc(confDevice->sfskStatus()));
}

void PreparingDeviceWindow::doDailyReport() {
    emit sendSequenceToDevice(countCrc->countCrc(confDevice->doDailyRep()));
    emit sendSequenceToDevice(countCrc->countCrc(confDevice->sfskStatus()));
}

void PreparingDeviceWindow::hideWindow() {
    hideMonitor();
}

void PreparingDeviceWindow::closeEvent(QCloseEvent *event) {
    event->ignore();
    hideMonitor();
}
