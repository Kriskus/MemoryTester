#include "preparingdevicewindow.h"
#include "ui_preparingdevicewindow.h"

PreparingDeviceWindow::PreparingDeviceWindow(QWidget *parent, bool currentStatusConnection) :
    QMainWindow(parent),
    ui(new Ui::PreparingDeviceWindow),
    statusConnection(currentStatusConnection) {
    ui->setupUi(this);
    countCrc = new CounterCrc();
    confDevice = new DeviceConfiguration();

    connect(ui->pushButtonClose, &QPushButton::clicked, this, &PreparingDeviceWindow::hideWindow);
    connect(ui->pushButtonHeader, &QPushButton::clicked, this, &PreparingDeviceWindow::setHeader);
    connect(ui->pushButtonVatRates, &QPushButton::clicked, this, &PreparingDeviceWindow::setVatRates);
    connect(ui->pushButtonDailyReport, &QPushButton::clicked, this, &PreparingDeviceWindow::doDailyReport);

    ui->textBrowserStatic->append("Tryb fiskalny      ");
    ui->textBrowserStatic->append("Zerowania RAM      ");
    ui->textBrowserStatic->append("Raporty dobowe     ");
    ui->textBrowserStatic->append("Ilość zmian stawek ");
    ui->textBrowserStatic->append("Stawka A           ");
    ui->textBrowserStatic->append("Stawka B           ");
    ui->textBrowserStatic->append("Stawka C           ");
    ui->textBrowserStatic->append("Stawka D           ");
    ui->textBrowserStatic->append("Stawka E           ");
    ui->textBrowserStatic->append("Stawka F           ");
    ui->textBrowserStatic->append("Stawka G           ");
    ui->textBrowserStatic->append("Data ostatniego RD ");
    ui->textBrowserStatic->append("Numer unikatowy    ");

    emit sendSequenceToDevice(countCrc->countCrc(confDevice->sfskStatus()));
}

PreparingDeviceWindow::~PreparingDeviceWindow() {
    delete ui;
    delete countCrc;
    delete confDevice;
}

void PreparingDeviceWindow::getStatusConnection(bool currentStatus) {
    statusConnection = currentStatus;
}

void PreparingDeviceWindow::setDeviceInformation(QByteArray data) {
    qApp->processEvents();
    ui->textBrowserResponse->clear();
    QList<QByteArray> dataSplitted = data.split('\t');
    dataSplitted.removeFirst();
    dataSplitted.removeLast();
    foreach (data, dataSplitted) {
        ui->textBrowserResponse->append(data.remove(0, 2));
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
