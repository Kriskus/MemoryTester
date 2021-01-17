#include "testerwindow.h"
#include "ui_testerwindow.h"

TesterWindow::TesterWindow(QWidget *parent, bool currentStatus) :
    QMainWindow(parent),
    ui(new Ui::TesterWindow) {
    ui->setupUi(this);
    connect(ui->pushButtonClose, &QPushButton::clicked, this, &TesterWindow::hideWindow);
    connect(ui->pushButtonStart, &QPushButton::clicked, this, &TesterWindow::startTestingThread);
    connect(ui->pushButtonStop, &QPushButton::clicked, this, &TesterWindow::stopThread);
    statusConnection = currentStatus;
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

void TesterWindow::getLastNumbers(int trNum, int fvNum) {
    ui->spinBoxTr->setValue(trNum);
    ui->spinBoxFv->setValue(fvNum);
    ui->checkBoxOther->setChecked(true);
}

void TesterWindow::startTestingThread() {
    if(statusConnection) {
        if(!threadInProgress) {
            QThread *testingThread;
            TesterOperation *testOper;
            testingThread = new QThread();
            if(ui->checkBoxOther->isChecked()) {
                testOper = new TesterOperation(nullptr, ui->spinBoxRepeat->value(), ui->checkBoxRememberNumbers->isChecked(), ui->radioButtonFvOld->isChecked(), ui->checkBoxDailyReport->isChecked(), ui->spinBoxTr->value(), ui->spinBoxFv->value());
            } else {
                testOper = new TesterOperation(nullptr, ui->spinBoxRepeat->value(), ui->checkBoxRememberNumbers->isChecked(), ui->radioButtonFvOld->isChecked(),ui->checkBoxDailyReport->isChecked());
            }

            connect(testingThread, &QThread::started, testOper, &TesterOperation::startoperations);
            connect(testOper, &TesterOperation::sendSequenceToDevice, this, &TesterWindow::getSequenceToSend, Qt::DirectConnection);
            connect(testOper, &TesterOperation::sendCurrentRepeat, this, &TesterWindow::showCurrentRepeat, Qt::DirectConnection);
            connect(this, &TesterWindow::stopThread, testOper, &TesterOperation::stopOperation, Qt::DirectConnection);
            connect(testOper, &TesterOperation::sendCurrentNumbers, this, &TesterWindow::getLastNumbers);
            connect(testOper, &TesterOperation::finished, testingThread, &QThread::quit);
            connect(testingThread, &QThread::finished, testingThread, &QThread::deleteLater);
            connect(testOper, &TesterOperation::finished, testOper, &TesterOperation::deleteLater);

            testingThread->start();
        } else {
            emit sendMessageBoxInformation("Operacja w trakcie...");
        }
    } else {
        emit sendMessageBoxInformation("Brak połączenia z urządzeniem!");
    }
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
