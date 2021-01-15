#include "datamonitor.h"
#include "ui_datamonitor.h"

DataMonitor::DataMonitor(QWidget *parent) : QMainWindow(parent), ui(new Ui::DataMonitor) {
    ui->setupUi(this);

    connect(ui->pushButtonClose, &QPushButton::clicked, this, &DataMonitor::hideWindow);
    connect(ui->pushButtonClear, &QPushButton::clicked, ui->textBrowserMonitor, &QTextBrowser::clear);

    connect(ui->pushButtonFontPlus, &QPushButton::clicked, this, &DataMonitor::increaseFontSize);
    connect(ui->pushButtonFontMinus, &QPushButton::clicked, this, &DataMonitor::decreaseFontSize);

    fontBold.setBold(true);
    ui->textBrowserMonitor->setCurrentFont(fontBold);
    ui->labelFontSize->setText(QString::number(fontBold.pointSize()));
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
    ui->textBrowserMonitor->append(time + "  |  " + data);
}

void DataMonitor::getTimeDiff(qint64 timeDiff) {
    qApp->processEvents();
    if(timeDiff > 50)
        ui->textBrowserMonitor->setTextColor(Qt::red);
    else
        ui->textBrowserMonitor->setTextColor(Qt::blue);
    ui->textBrowserMonitor->append("\nCzas oczekiwania na odpowiedź: " + QString::number(timeDiff) + " ms\n--------------\n\n");
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

