#include "testerwindow.h"
#include "ui_testerwindow.h"

TesterWindow::TesterWindow(QWidget *parent, bool currentStatus) :
    QMainWindow(parent),
    ui(new Ui::TesterWindow) {
    ui->setupUi(this);

    connect(ui->pushButtonClose, &QPushButton::clicked, this, &TesterWindow::hideWindow);
    connect(ui->pushButtonStart, &QPushButton::clicked, this, &TesterWindow::startTestingThread);
    connect(ui->pushButtonStop, &QPushButton::clicked, this, &TesterWindow::stopThread);
}

TesterWindow::~TesterWindow() {
    delete ui;
}

void TesterWindow::getConnectionStatus(bool currentStatus) {
    qApp->processEvents();
    statusConnection = currentStatus;
}

void TesterWindow::getSequenceToSend(QByteArray data) {
    emit sendSequenceToDevice(data);
}

void TesterWindow::startTestingThread() {
    QThread *testingThread;
    TesterOperation *testOper;
    testingThread = new QThread();
    testOper = new TesterOperation(nullptr, ui->spinBoxRepeat->value());

    connect(testingThread, &QThread::started, testOper, &TesterOperation::startoperations);
    connect(testOper, &TesterOperation::sendSequenceToDevice, this, &TesterWindow::getSequenceToSend, Qt::DirectConnection);
    connect(testOper, &TesterOperation::sendCurrentRepeat, this, &TesterWindow::showCurrentRepeat, Qt::DirectConnection);
    connect(this, &TesterWindow::stopThread, testOper, &TesterOperation::stopOperation, Qt::DirectConnection);
    connect(testOper, &TesterOperation::finished, testingThread, &QThread::quit);
    connect(testingThread, &QThread::finished, testingThread, &QThread::deleteLater);
    connect(testOper, &TesterOperation::finished, testOper, &TesterOperation::deleteLater);

    testingThread->start();
}

void TesterWindow::stopTestingThread() {
    emit stopThread();
}

void TesterWindow::showCurrentRepeat(int currentRepeat) {
    ui->labelCurrentRepeat->setText(QString::number(currentRepeat));
}

void TesterWindow::hideWindow() {
    emit hideMonitor();
}

void TesterWindow::closeEvent(QCloseEvent *event) {
    event->ignore();
}
