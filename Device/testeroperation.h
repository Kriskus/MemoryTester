#ifndef TESTEROPERATION_H
#define TESTEROPERATION_H

#include <QObject>
#include <QThread>
#include <QApplication>
#include <QTimer>

#include "devicedatabase.h"
#include "devicetransaction.h"
#include "deviceconfiguration.h"
#include "Other/countercrc.h"

class TesterOperation : public QObject
{
    Q_OBJECT
public:
    explicit TesterOperation(QObject *parent = nullptr, int rep = 0, bool remember = false, bool fvT = false, bool doRD = "false", int trNumBeg = 0, int fvNumBeg = 0);

public slots:
    void startoperations();
    void stopOperation();

private:
    DeviceDataBase *dbDevice;
    DeviceTransaction *trDevice;
    DeviceConfiguration *confDevice;
    CounterCrc *countCrc;
    QTimer *sprnTimer;

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
    bool fvNew{false};
    bool dailyReport{false};

private slots:
    void initTr();
    void initFv();
    void addTrLines();
    void addFvLines();
    void endPrint();
    void chkPrnStat();

signals:
    void sendSequenceToDevice(QByteArray);
    void sendSequenceToDeviceAsyn(QByteArray);
    void sendCurrentRepeat(int);
    void sendCurrentNumbers(int, int);
    void finished();
};

#endif // TESTEROPERATION_H
