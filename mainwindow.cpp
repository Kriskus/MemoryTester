#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    serial = new QSerialPort(this);
    socket = new QTcpSocket(this);
    timerResponse = new QTimer(this);
    prepareDataMonitorWindow();
    prepareTesterWindow();
    showAvailableDevices();

    connect(ui->pushButtonExit, &QPushButton::clicked, this, &MainWindow::close);
    connect(ui->pushButtonDataMonitor, &QPushButton::clicked, this, &MainWindow::showDataMonitorWindow);
    connect(ui->pushButtonMemoryTester, &QPushButton::clicked, this, &MainWindow::showTesterWindow);

    connect(ui->pushButtonConnect_1, &QPushButton::clicked, this, &MainWindow::deviceConnect);
    connect(ui->pushButtonDisconnect_1, &QPushButton::clicked, this, &MainWindow::deviceDisconnect);
    connect(ui->pushButtonRefresh_1, &QPushButton::clicked, this, &MainWindow::showAvailableDevices);

    connect(this, &MainWindow::sendMessageBoxInformation, this, &MainWindow::showMessageBoxInformation);
    connect(this, &MainWindow::sendStatusBarInformation, this, &MainWindow::showStatusBarInformation);

    logThread();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::deviceConnect() {
    if(!connectionStatus) {
        if(ui->radioButtonCom_1->isChecked()) serialConnect();
        else socketConnect();
    }
}

void MainWindow::deviceDisconnect() {
    if(connectionStatus) {
        if(ui->radioButtonCom_1->isChecked()) serialDisconnect();
        else socketDisconnect();
    }
}

void MainWindow::deviceReadWrite(QByteArray data) {
    if(connectionStatus) {
        if(ui->radioButtonCom_1->isChecked()) serialReadWrite(data);
        else socketReadWrite(data);
    }
}

void MainWindow::serialConnect() {
    serial->setPortName(ui->comboBoxPortName_1->currentText().split(" ").first());
    serial->setBaudRate(ui->comboBoxConnectionSpeed_1->currentText().toInt());
    serial->setDataBits(QSerialPort::Data8);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setParity(QSerialPort::NoParity);
    if(serial->open(QIODevice::ReadWrite)) {
        ui->labelConnectionColor->setStyleSheet("QLabel { background: green; border-radius: 8px;}");
        ui->labelConnectionStatus->setText("Połączono z " + ui->comboBoxPortName_1->currentText().split(" ").first());
        emit sendConnectionStatusDevice(connectionStatus = true);
    } else {
        emit sendMessageBoxInformation(serial->errorString());
    }
}

void MainWindow::serialDisconnect() {
    serial->close();
    ui->labelConnectionColor->setStyleSheet("QLabel { background: red; border-radius: 8px;}");
    ui->labelConnectionStatus->setText("Rozłączone");
    emit sendConnectionStatusDevice(connectionStatus = false);
}

void MainWindow::serialReadWrite(QByteArray data) {
    serial->write(data);
    timeWrite = QDateTime::currentDateTime();
    serial->waitForBytesWritten(10);
    response.clear();
    timerResponse->start(10000);
    while(!response.contains("\03") && timerResponse->remainingTime() > 0) {
        response.append(serial->readAll());
        serial->waitForReadyRead(1);
        if(timerResponse->remainingTime() < 0)
            emit sendStatusBarInformation("Brak odpowiedzi z urządzenia");
    }
    timeRead = QDateTime::currentDateTime();
    emit sendWritedData(timeWrite.toString("yyyy-MM-dd HH:mm:ss.zzz").toLatin1(), data);
    emit sendReadedData(timeRead.toString("yyyy-MM-dd HH:mm:ss.zzz").toLatin1(), response);
    emit sendTimeDifference(timeWrite.msecsTo(timeRead));
}

void MainWindow::socketConnect() {
    socket->connectToHost(ui->lineEditIp_1->text(), ui->lineEditPort_1->text().toInt());
    socket->open(QIODevice::ReadWrite);
    socket->waitForConnected();
    if(socket->isOpen()) {
        ui->labelConnectionColor->setStyleSheet("QLabel { color : green; }");
        ui->labelConnectionStatus->setText("Połączono z " + ui->lineEditIp_1->text());
        emit sendConnectionStatusDevice(connectionStatus = true);
    }
}

void MainWindow::socketDisconnect() {
    socket->close();
    ui->labelConnectionColor->setStyleSheet("QLabel { background: red; border-radius: 8px;}");
    ui->labelConnectionStatus->setText("Rozłączone");
    emit sendConnectionStatusDevice(connectionStatus = false);
}

void MainWindow::socketReadWrite(QByteArray data) {
    socket->write(data);
    timeWrite = QDateTime::currentDateTime();
    socket->waitForBytesWritten(10);
    response.clear();
    timerResponse->start(10000);
    while(!response.contains("\03") && timerResponse->remainingTime() > 0) {
        response.append(socket->readAll());
        socket->waitForReadyRead(1);
        if(timerResponse->remainingTime() < 0)
            emit sendStatusBarInformation("Brak odpowiedzi z urządzenia");
    }
    timeRead = QDateTime::currentDateTime();
    emit sendWritedData(timeWrite.toString("yyyy-MM-dd HH:mm:ss.zzz").toLatin1(), data);
    emit sendReadedData(timeRead.toString("yyyy-MM-dd HH:mm:ss.zzz").toLatin1(), response);
    emit sendTimeDifference(timeWrite.msecsTo(timeRead));
}

void MainWindow::showAvailableDevices() {
    ui->comboBoxPortName_1->clear();
    foreach (const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts()) {
        ui->comboBoxPortName_1->addItem(serialPortInfo.portName() + " | " + serialPortInfo.manufacturer() + " | " + serialPortInfo.description());
    }
}

void MainWindow::showMessageBoxInformation(QString information) {
    QMessageBox::information(this, "Uwaga", information, QMessageBox::Ok);
}

void MainWindow::showStatusBarInformation(QString information) {
    ui->statusbar->showMessage(information, 2000);
}

void MainWindow::prepareDataMonitorWindow() {
    dataMonitor = new DataMonitor();
    connect(this, &MainWindow::sendReadedData, dataMonitor, &DataMonitor::getReadedData, Qt::DirectConnection);
    connect(this, &MainWindow::sendWritedData, dataMonitor, &DataMonitor::getSendedData, Qt::DirectConnection);
    connect(this, &MainWindow::sendTimeDifference, dataMonitor, &DataMonitor::getTimeDiff, Qt::DirectConnection);
    connect(dataMonitor, &DataMonitor::hideMonitor, dataMonitor, &DataMonitor::hide);
    connect(dataMonitor, &DataMonitor::hideMonitor, this, &MainWindow::changeDataMonitorWindowStatus);
    connect(this, &MainWindow::finished, dataMonitor, &DataMonitor::deleteLater);
}

void MainWindow::showDataMonitorWindow() {
    if(!dataMonitorWindowStatus) {
        dataMonitor->show();
        dataMonitorWindowStatus = true;
    }
}

void MainWindow::changeDataMonitorWindowStatus() {
    dataMonitorWindowStatus = false;
}

void MainWindow::prepareTesterWindow() {
    testerMonitor = new TesterWindow(nullptr, connectionStatus);
    connect(testerMonitor, &TesterWindow::sendSequenceToDevice, this, &MainWindow::deviceReadWrite, Qt::DirectConnection);
    connect(testerMonitor, &TesterWindow::sendMessageBoxInformation, this, &MainWindow::showMessageBoxInformation, Qt::DirectConnection);
    connect(this, &MainWindow::sendConnectionStatusDevice, testerMonitor, &TesterWindow::getConnectionStatus, Qt::DirectConnection);
    connect(testerMonitor, &TesterWindow::hideMonitor, testerMonitor, &TesterWindow::hide);
    connect(testerMonitor, &TesterWindow::hideMonitor, this, &MainWindow::changeTesterWindowStatus);
    connect(this, &MainWindow::finished, testerMonitor, &TesterWindow::deleteLater);
}

void MainWindow::showTesterWindow() {
    if(!testerWindowStatus) {
        testerMonitor->show();
        testerWindowStatus = true;
    }
}

void MainWindow::changeTesterWindowStatus() {
    testerWindowStatus = false;
}

void MainWindow::logThread() {
    QThread *loggingThread;
    LogData *logData;

    loggingThread = new QThread(this);
    logData = new LogData(nullptr, QDateTime::currentDateTime().toString("Log/Log_yyyy_MM_dd_HH_mm_ss_zzz") + ".txt");

    connect(this, &MainWindow::sendWritedData, logData, &LogData::appendLogFile);
    connect(this, &MainWindow::sendReadedData, logData, &LogData::appendLogFile);
    connect(logData, &LogData::sendMessageBoxInformation, this, &MainWindow::showMessageBoxInformation);

    connect(logData, &LogData::finished, loggingThread, &QThread::quit);
    connect(logData, &LogData::finished, logData, &LogData::deleteLater);
    connect(this, &MainWindow::finished, loggingThread, &QThread::quit);
    connect(loggingThread, &QThread::finished, loggingThread, &QThread::deleteLater);
    connect(this, &MainWindow::finished, logData, &LogData::deleteLater);

    loggingThread->start();
}

void MainWindow::closeWindow() {
    emit finished();
}

void MainWindow::closeEvent(QCloseEvent *event) {
    emit finished();
    event->accept();
}