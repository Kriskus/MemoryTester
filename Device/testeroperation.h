#ifndef TESTEROPERATION_H
#define TESTEROPERATION_H

#include <QObject>
#include <QThread>
#include <QApplication>

#include "devicedatabase.h"
#include "devicetransaction.h"
#include "Other/countercrc.h"

class TesterOperation : public QObject
{
    Q_OBJECT
public:
    explicit TesterOperation(QObject *parent = nullptr, int rep = 0, bool remember = false, bool fvT = false, int trNumBeg = 0, int fvNumBeg = 0);

public slots:
    void startoperations();
    void stopOperation();

private:
    DeviceDataBase *dbDevice;
    DeviceTransaction *trDevice;
    CounterCrc *countCrc;

    int currentLine;
    int repeats;
    int currentRepeat;
    int currentTrLine;
    int currentFvLine;
    bool increaceTr{false};
    bool increaceFv{false};
    bool currentTrType{true};
    bool activeThread{false};
    bool rememberNumbers{false};
    bool fvType{false};

private slots:
    void initTr();
    void initFv();
    void addTrLines();
    void addFvLines();
    void endPrint();

signals:
    void sendSequenceToDevice(QByteArray);
    void sendCurrentRepeat(int);
    void sendCurrentNumbers(int, int);
    void finished();
};

#endif // TESTEROPERATION_H
