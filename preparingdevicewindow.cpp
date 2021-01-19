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

    ui->textBrowserStaticSfsk->append("Tryb fiskalny      ");
    ui->textBrowserStaticSfsk->append("Zerowania RAM      ");
    ui->textBrowserStaticSfsk->append("Raporty dobowe     ");
    ui->textBrowserStaticSfsk->append("Ilość zmian stawek ");
    ui->textBrowserStaticSfsk->append("Stawka A           ");
    ui->textBrowserStaticSfsk->append("Stawka B           ");
    ui->textBrowserStaticSfsk->append("Stawka C           ");
    ui->textBrowserStaticSfsk->append("Stawka D           ");
    ui->textBrowserStaticSfsk->append("Stawka E           ");
    ui->textBrowserStaticSfsk->append("Stawka F           ");
    ui->textBrowserStaticSfsk->append("Stawka G           ");
    ui->textBrowserStaticSfsk->append("Data ostatniego RD ");
    ui->textBrowserStaticSfsk->append("Numer unikatowy    ");

    ui->textBrowserStaticScnt->append("Licznik RD");
    ui->textBrowserStaticScnt->append("Nr ostatniego nagłówka");
    ui->textBrowserStaticScnt->append("Licznik paragonów");
    ui->textBrowserStaticScnt->append("Licznik faktur");
    ui->textBrowserStaticScnt->append("Nr unikatowy");
    ui->textBrowserStaticScnt->append("Licznik paragonów anulowanych");
    ui->textBrowserStaticScnt->append("Nr ostatniego paragonu");
    ui->textBrowserStaticScnt->append("Licznik faktur anulowanych");

    emit sendSequenceToDevice(countCrc->countCrc(confDevice->sfskStatus()));
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

void PreparingDeviceWindow::setDeviceInformationSfsk(QByteArray data) {
    qApp->processEvents();
    ui->textBrowserResponseSfsk->clear();
    QList<QByteArray> dataSplitted = data.split('\t');
    dataSplitted.removeFirst();
    dataSplitted.removeLast();
    foreach (data, dataSplitted) {
        ui->textBrowserResponseSfsk->append(data.remove(0, 2));
    }
}

void PreparingDeviceWindow::setDeviceInformationScnt(QByteArray data) {
    qApp->processEvents();
    ui->textBrowserResponseScnt->clear();
    QList<QByteArray> dataSplitted = data.split('\t');
    dataSplitted.removeFirst();
    dataSplitted.removeLast();
    foreach (data, dataSplitted) {
        ui->textBrowserResponseScnt->append(data.remove(0, 2));
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
