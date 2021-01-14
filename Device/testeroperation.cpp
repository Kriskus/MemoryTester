#include "testeroperation.h"

TesterOperation::TesterOperation(QObject *parent, int rep) : QObject(parent), repeats(rep) {
    currentTrRepeat = 0;
    currentFvRepeat = 0;
    currentRepeat = 0;
    dbDevice = new DeviceDataBase();
    trDevice = new DeviceTransaction();
    countCrc = new CounterCrc();
}

void TesterOperation::startoperations() {
    activeThread = true;
    while(currentRepeat < repeats && activeThread) {
        emit sendCurrentRepeat(currentRepeat + 1);
        if(currentTrType) {
            initTr();
        } else {
            initFv();
        }
        if(currentTrType) {
            addTrLines();
        } else {
            addFvLines();
        }
        endPrint();
    }
    emit sendSequenceToDevice(countCrc->countCrc("beep\t"));
    emit finished();
}

void TesterOperation::stopOperation() {
    qApp->processEvents();
    activeThread = false;
}

void TesterOperation::initTr() {
    emit sendSequenceToDevice(countCrc->countCrc(trDevice->transactionInit()));
}

void TesterOperation::initFv() {
    emit sendSequenceToDevice(countCrc->countCrc(trDevice->invoiceInit()));
    emit sendSequenceToDevice(countCrc->countCrc(trDevice->invoiceBuyer()));
    emit sendSequenceToDevice(countCrc->countCrc(trDevice->invoiceNumber()));
}

void TesterOperation::addTrLines() {
    for(int rep = 0; rep < 500; rep++) {
        emit sendSequenceToDevice(countCrc->countCrc(trDevice->transactionTrLine(currentTrRepeat, rep%7)));
        emit sendSequenceToDevice(countCrc->countCrc(dbDevice->dbCheckTrLine(rep%7, currentTrRepeat)));
        if(increaceTr) {
            currentTrRepeat++;
            increaceTr = false;
        } else {
            increaceTr = true;
        }
    }
}

void TesterOperation::addFvLines() {
    for(int rep = 0; rep < 120; rep++) {
        emit sendSequenceToDevice(countCrc->countCrc(trDevice->transactionInLine(currentFvRepeat, rep%7)));
        emit sendSequenceToDevice(countCrc->countCrc(dbDevice->dbCheckTrLine(rep%7, currentFvRepeat)));
        if(increaceFv) {
            currentFvRepeat++;
            increaceFv = false;
        } else {
            increaceFv = true;
        }
    }
}

void TesterOperation::endPrint() {
    if(currentTrType) {
        emit sendSequenceToDevice(countCrc->countCrc(trDevice->transactionTrEnd()));
        currentTrType = false;
    } else {
        emit sendSequenceToDevice(countCrc->countCrc(trDevice->transactionFvEnd()));
        currentTrType = true;
        currentRepeat++;
    }
}
