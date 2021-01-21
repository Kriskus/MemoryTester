#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    serial = new QSerialPort(this);
    socket = new QTcpSocket(this);
    devConf = new DeviceConfiguration();
    dataMonitor = new DataMonitor();
    testerMonitor = new TesterWindow(nullptr, connectionStatus);
    configWindow = new PreparingDeviceWindow(nullptr, connectionStatus);
    countCrc = new CounterCrc();

    prepareDataMonitorWindow();
    prepareTesterWindow();
    prepareConfWindow();
    showAvailableDevices();

    connect(ui->pushButtonExit, &QPushButton::clicked, this, &MainWindow::close);
    connect(ui->pushButtonDataMonitor, &QPushButton::clicked, this, &MainWindow::showDataMonitorWindow);
    connect(ui->pushButtonMemoryTester, &QPushButton::clicked, this, &MainWindow::showTesterWindow);
    connect(ui->pushButtonConfiguration, &QPushButton::clicked, this, &MainWindow::showConfWindow);

    connect(ui->pushButtonConnect_1, &QPushButton::clicked, this, &MainWindow::deviceConnect);
    connect(ui->pushButtonDisconnect_1, &QPushButton::clicked, this, &MainWindow::deviceDisconnect);
    connect(ui->pushButtonRefresh_1, &QPushButton::clicked, this, &MainWindow::showAvailableDevices);

    connect(this, &MainWindow::sendMessageBoxInformation, this, &MainWindow::showMessageBoxInformation);
    connect(this, &MainWindow::sendStatusBarInformation, this, &MainWindow::showStatusBarInformation);
    connect(this, &MainWindow::sendSequence, this, &MainWindow::deviceReadWrite);

    connect(ui->labelVersion, &QLabel::linkActivated, this, &MainWindow::showVersionInformation);
    ui->labelVersion->setText("<a href>Wersja</a>");
    ui->labelConnectionColor->setStyleSheet("QLabel { background: red; border-radius: 8px;}");
    logThread();
    countThread();
}

MainWindow::~MainWindow() {
    delete ui;
    delete serial;
    delete socket;
    delete devConf;
    delete testerMonitor;
    delete dataMonitor;
    delete configWindow;
    delete countCrc;
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
    switch (ui->comboBoxFlowControl_1->currentIndex()) {
    case 0: serial->setFlowControl(QSerialPort::NoFlowControl); break;
    case 1: serial->setFlowControl(QSerialPort::SoftwareControl); break;
    case 2: serial->setFlowControl(QSerialPort::HardwareControl); break;
    default: break;
    }
    serial->setStopBits(QSerialPort::OneStop);
    switch (ui->comboBoxParity_1->currentIndex()) {
    case 0: serial->setParity(QSerialPort::NoParity); break;
    case 1: serial->setParity(QSerialPort::EvenParity); break;
    case 2: serial->setParity(QSerialPort::OddParity); break;
    default: break;
    }
    if(serial->open(QIODevice::ReadWrite)) {
        ui->labelConnectionColor->setStyleSheet("QLabel { background: green; border-radius: 8px;}");
        ui->labelConnectionStatus->setText("Połączono z " + ui->comboBoxPortName_1->currentText().split(" ").first());
        emit sendConnectionStatusDevice(connectionStatus = true);
        emit sendSequence(countCrc->countCrc(devConf->sfskStatus()));
        emit sendSequence(countCrc->countCrc(devConf->scntStatus()));
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
    response.clear();
    while(!response.contains("\03")) {
        response.append(serial->readAll());
        serial->waitForReadyRead(1);
    }
    timeRead = QDateTime::currentDateTime();
    emit sendWritedData(timeWrite.toString("yyyy-MM-dd HH:mm:ss.zzz").toLatin1(), data);
    emit sendReadedData(timeRead.toString("yyyy-MM-dd HH:mm:ss.zzz").toLatin1(), response);
    if(response.contains("sfsk"))
        emit sendSfskStatus(response);
    if(response.contains("trend")) {
        emit sendTimeDifference(0);
        emit sendSequence(countCrc->countCrc(devConf->scntStatus()));
    } else {
        emit sendTimeDifference(timeWrite.msecsTo(timeRead));
    }
    if(response.contains("scnt"))
        emit sendScntStatus(response);
}

void MainWindow::socketConnect() {
    socket->connectToHost(ui->lineEditIp_1->text(), ui->lineEditPort_1->text().toInt());
    socket->open(QIODevice::ReadWrite);
    socket->waitForConnected();
    if(socket->isOpen()) {
        ui->labelConnectionColor->setStyleSheet("QLabel { background: green; border-radius: 8px;}");
        ui->labelConnectionStatus->setText("Połączono z " + ui->lineEditIp_1->text());
        emit sendConnectionStatusDevice(connectionStatus = true);
        emit sendSequence(countCrc->countCrc(devConf->sfskStatus()));
        emit sendSequence(countCrc->countCrc(devConf->scntStatus()));
    } else {
        emit sendMessageBoxInformation(socket->errorString());
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
    response.clear();
    while(!response.contains("\03")) {
        response.append(socket->readAll());
        socket->waitForReadyRead(1);
    }
    timeRead = QDateTime::currentDateTime();
    emit sendWritedData(timeWrite.toString("yyyy-MM-dd HH:mm:ss.zzz").toLatin1(), data);
    emit sendReadedData(timeRead.toString("yyyy-MM-dd HH:mm:ss.zzz").toLatin1(), response);
    if(response.contains("sfsk"))
        emit sendSfskStatus(response);
    if(response.contains("trend")) {
        emit sendTimeDifference(0);
        emit sendSequence(countCrc->countCrc(devConf->scntStatus()));
    } else {
        emit sendTimeDifference(timeWrite.msecsTo(timeRead));
    }
    if(response.contains("scnt"))
        emit sendScntStatus(response);
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
    connect(testerMonitor, &TesterWindow::sendSequenceToDevice, this, &MainWindow::deviceReadWrite, Qt::DirectConnection);
    connect(testerMonitor, &TesterWindow::sendMessageBoxInformation, this, &MainWindow::showMessageBoxInformation, Qt::DirectConnection);
    connect(this, &MainWindow::sendConnectionStatusDevice, testerMonitor, &TesterWindow::getConnectionStatus, Qt::DirectConnection);
    connect(testerMonitor, &TesterWindow::endTest, dataMonitor, &DataMonitor::endTest, Qt::DirectConnection);
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

void MainWindow::prepareConfWindow() {
    connect(configWindow, &PreparingDeviceWindow::sendSequenceToDevice, this, &MainWindow::deviceReadWrite, Qt::DirectConnection);
    connect(this, &MainWindow::sendConnectionStatusDevice, configWindow, &PreparingDeviceWindow::getStatusConnection, Qt::DirectConnection);
    connect(this, &MainWindow::sendSfskStatus, configWindow, &PreparingDeviceWindow::setDeviceInformationSfsk, Qt::DirectConnection);
    connect(this, &MainWindow::sendScntStatus, configWindow, &PreparingDeviceWindow::setDeviceInformationScnt, Qt::DirectConnection);
    connect(configWindow, &PreparingDeviceWindow::hideMonitor, configWindow, &TesterWindow::hide);
    connect(configWindow, &PreparingDeviceWindow::hideMonitor, this, &MainWindow::changeConfWindowStatus);
    connect(this, &MainWindow::finished, configWindow, &PreparingDeviceWindow::deleteLater);
}

void MainWindow::showConfWindow() {
    if(!configWindowStatus) {
        configWindow->show();
        configWindowStatus = true;
    }
}

void MainWindow::changeConfWindowStatus() {
    configWindowStatus = false;
}

void MainWindow::logThread() {
    QThread *loggingThread;
    LogData *logData;
    loggingThread = new QThread(this);
    logData = new LogData(nullptr, "Logs/Log_" + QDateTime::currentDateTime().toString("yyyy_MM_dd_HH_mm_ss_zzz") + ".txt");

    connect(this, &MainWindow::sendWritedData, logData, &LogData::appendLogFile);
    connect(this, &MainWindow::sendReadedData, logData, &LogData::appendLogFile);
    connect(this, &MainWindow::sendTimeDifference, logData, &LogData::convertToQByteArray);
    connect(logData, &LogData::sendMessageBoxInformation, this, &MainWindow::showMessageBoxInformation);

    connect(logData, &LogData::finished, loggingThread, &QThread::quit);
    connect(logData, &LogData::finished, logData, &LogData::deleteLater);
    connect(this, &MainWindow::finished, loggingThread, &QThread::quit);
    connect(loggingThread, &QThread::finished, loggingThread, &QThread::deleteLater);
    connect(this, &MainWindow::finished, logData, &LogData::deleteLater);

    loggingThread->start();
}

void MainWindow::countThread() {
        QThread *timeCounterThread;
        CounterTimer *countTime;
        timeCounterThread = new QThread(this);
        countTime = new CounterTimer();
        countTime->moveToThread(timeCounterThread);

        connect(this, &MainWindow::sendTimeDifference, countTime, &CounterTimer::countTime, Qt::DirectConnection);
        connect(dataMonitor, &DataMonitor::requestClearTime, countTime, &CounterTimer::clearTimes, Qt::DirectConnection);
        connect(countTime, &CounterTimer::sendCurrentAverageTime, dataMonitor, &DataMonitor::showAverageTime, Qt::DirectConnection);
        connect(this, &MainWindow::finished, timeCounterThread, &QThread::quit);
        connect(timeCounterThread, &QThread::finished, timeCounterThread, &QThread::deleteLater);
        connect(this, &MainWindow::finished, countTime, &CounterTimer::deleteLater);

        timeCounterThread->start();
}

void MainWindow::showVersionInformation() {
    QMessageBox::information(this, "Historia wersji",
                                                      "Wersja 0.9.91\n"
                                                      "    - poprawione zliczanie średniego czasu odpowiedzi - całkowite ignorowanie [trend]\n"
                                                      "\n"
                                                      "Wersja 0.9.9\n"
                                                      "    - dodana informacja o średnim czasie odpowiedzi\n"
                                                      "    - dla zakończenia paragonu i faktur czas odpowiedzi został ustawiony na 1 ms\n"
                                                      "\n"
                                                      "Wersja 0.9.8\n"
                                                      "    - dodana numeracja bloków\n"
                                                      "\n"
                                                      "Wersja 0.9.8\n"
                                                      "    - dodany odczyt statusu scnt\n"
                                                      "    - dodana informacja o różnicy w czasie w pliku log\n"
                                                      "\n"
                                                      "Wersja 0.9.7\n"
                                                      "    - dodany raport dobowy\n"
                                                      "    - dodane programowanie stawek VAT\n"
                                                      "    - dodane programowanie nagłówka\n"
                                                      "    - dodane drukowanie raportu dobowego po zakończeniu testu\n"
                                                      "    - usunięty timeout przy odczytywaniu odpowiedzi z drukarki\n"
                                                      "              (przy błędzie może odczytywać w nieskończoność\n"
                                                      "\n"
                                                      "Wersja 0.9.6\n"
                                                      "    - poprawione rozpoczęcie faktury dla EJ\n"
                                                      "\n"
                                                      "Wersja 0.9.5\n"
                                                      "    - poprawiono wyświetlanie połączenie po TCP\n"
                                                      "    - zwiękoszono maksymalną liczbę początkową faktur i paragonów\n"
                                                      "    - dodano sprawdzanie czy istnieje katalog z logami i ewentualne utworzenie go\n"
                                                      "\n"
                                                      "Wersja 0.9.4\n"
                                                      "    - dodano rozróżnienie faktur Online/EJ\n"
                                                      "    - dodano możliwość ustawienia początkowych numerów w nazwach pozycji\n"
                                                      "    - dodano możliwość zapamiętania ostatnich numerów po zakończonym teście\n"
                                                      "\n"
                                                      "Wersja 0.9.2\n"
                                                      "    - wersja początkowa\n\n", QMessageBox::Ok);
}

void MainWindow::closeWindow() {
    emit finished();
}

void MainWindow::closeEvent(QCloseEvent *event) {
    emit finished();
    event->accept();
}
