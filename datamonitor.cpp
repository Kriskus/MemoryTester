#include "datamonitor.h"
#include "ui_datamonitor.h"

DataMonitor::DataMonitor(QWidget *parent) : QMainWindow(parent), ui(new Ui::DataMonitor) {
    ui->setupUi(this);

    connect(ui->pushButtonClose, &QPushButton::clicked, this, &DataMonitor::hideWindow);
    connect(ui->pushButtonClear, &QPushButton::clicked, this, &DataMonitor::clearTextBrowser);

    connect(ui->pushButtonFontPlus, &QPushButton::clicked, this, &DataMonitor::increaseFontSize);
    connect(ui->pushButtonFontMinus, &QPushButton::clicked, this, &DataMonitor::decreaseFontSize);

    connect(this, &DataMonitor::increaseTimeout, this, &DataMonitor::showCurrentTimeoutCounter);

    fontBold.setBold(true);
    ui->textBrowserMonitor->setCurrentFont(fontBold);
    ui->labelFontSize->setText(QString::number(fontBold.pointSize()));
    currentTimeout = 0;
    ui->labelAmount->setText("0");
    ui->labelTimeAverage->setText("0");
}

DataMonitor::~DataMonitor() {
    delete ui;
}

void DataMonitor::getReadedData(QByteArray time, QByteArray data) {
    qApp->processEvents();
    if(data.contains("?")) showResponseError(time + "  |  " + data);
    else showResponse(time + "  |  " + data);
}

void DataMonitor::getSendedData(QByteArray time, QByteArray data) {
    qApp->processEvents();
    data.replace("\t", " ");
    ui->textBrowserMonitor->setTextColor(Qt::black);
    ui->textBrowserMonitor->append("#" + QString::number(currentBlock++));
    ui->textBrowserMonitor->append(time + "  |  " + data);
}

void DataMonitor::getTimeDiff(qint64 timeDiff) {
    qApp->processEvents();
    if(timeDiff > 50) {
        ui->textBrowserMonitor->setTextColor("#F9A602");
        emit increaseTimeout();
    } else
        ui->textBrowserMonitor->setTextColor(Qt::blue);
    ui->textBrowserMonitor->append("\nCzas oczekiwania na odpowiedź: " + QString::number(timeDiff) + " ms\n--------------\n\n");
}

void DataMonitor::clearTextBrowser() {
    ui->textBrowserMonitor->clear();
    ui->labelAmount->setText(QString::number(currentTimeout = 0));
}

void DataMonitor::showCurrentTimeoutCounter() {
    ui->labelAmount->setText(QString::number(++currentTimeout));
}

void DataMonitor::showResponse(QByteArray data) {
    data.replace("\t", " ");
    ui->textBrowserMonitor->setTextColor(Qt::blue);
    ui->textBrowserMonitor->append(data);
}

void DataMonitor::showResponseError(QByteArray data) {
    data.replace("\t", " ");
    ui->textBrowserMonitor->setTextColor(Qt::red);
    ui->textBrowserMonitor->append(data);
}

void DataMonitor::showAverageTime(int currAvrTime) {
    ui->labelTimeAverage->setText(QString::number(currAvrTime) + " ms");
}

void DataMonitor::endTest() {
    emit requestClearTime();
}

void DataMonitor::increaseFontSize() {
    fontBold.setPointSize(fontBold.pointSize()+1);
    ui->labelFontSize->setText(QString::number(fontBold.pointSize()));
}

void DataMonitor::decreaseFontSize() {
    fontBold.setPointSize(fontBold.pointSize()-1);
    ui->labelFontSize->setText(QString::number(fontBold.pointSize()));
}

void DataMonitor::hideWindow() {
    emit hideMonitor();
}

void DataMonitor::closeEvent(QCloseEvent *event) {
    event->ignore();
}

