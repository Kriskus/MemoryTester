#include "testeroperation.h"

TesterOperation::TesterOperation(QObject *parent, int rep, bool remember, bool fvT, bool doRD, int trNumBeg, int fvNumBeg) : QObject(parent), repeats(rep) {
    currentTrLine = 0;
    currentFvLine = 0;
    currentRepeat = 0;
    rememberNumbers = remember;
    currentTrLine = trNumBeg;
    currentFvLine = fvNumBeg;
    fvNew = fvT;
    if(repeats == 0) repeats = 1000000;

    dbDevice = new DeviceDataBase();
    trDevice = new DeviceTransaction();
    confDevice = new DeviceConfiguration();
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
            if(!activeThread)
                break;
        } else {
            addFvLines();
            if(!activeThread)
                break;
        }
        endPrint();
    }
    emit sendSequenceToDevice(countCrc->countCrc("beep\t"));
    if(rememberNumbers)
        emit sendCurrentNumbers(currentTrLine, currentFvLine);
    if(dailyReport)
        emit sendSequenceToDevice(countCrc->countCrc(confDevice->doDailyRep()));
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
    if(!fvNew) {
        emit sendSequenceToDevice(countCrc->countCrc(trDevice->invoiceOnlineInit()));
        emit sendSequenceToDevice(countCrc->countCrc(trDevice->invoiceBuyer()));
        emit sendSequenceToDevice(countCrc->countCrc(trDevice->invoiceNumber()));
    } else {
        emit sendSequenceToDevice(countCrc->countCrc(trDevice->invoiceEjInit()));
    }
}

void TesterOperation::addTrLines() {
    for(int rep = 0; rep < 500; rep++) {
        emit sendSequenceToDevice(countCrc->countCrc(trDevice->transactionTrLine(currentTrLine, rep%7)));
        emit sendSequenceToDevice(countCrc->countCrc(dbDevice->dbCheckTrLine(rep%7, currentTrLine)));
        if(increaceTr) {
            currentTrLine++;
            increaceTr = false;
        } else {
            increaceTr = true;
        }
        if(!activeThread)
            break;
    }
}

void TesterOperation::addFvLines() {
    for(int rep = 0; rep < 120; rep++) {
        emit sendSequenceToDevice(countCrc->countCrc(trDevice->transactionInLine(currentFvLine, rep%7)));
        emit sendSequenceToDevice(countCrc->countCrc(dbDevice->dbCheckFvLine(rep%7, currentFvLine)));
        if(increaceFv) {
            currentFvLine++;
            increaceFv = false;
        } else {
            increaceFv = true;
        }
        if(!activeThread)
            break;
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
