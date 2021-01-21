#ifndef DATAMONITOR_H
#define DATAMONITOR_H

#include <QMainWindow>
#include <QFont>
#include <QTextCursor>
#include <QCloseEvent>
#include <QDateTime>
#include <QThread>

#include "countertimer.h"

namespace Ui {
class DataMonitor;
}

class DataMonitor : public QMainWindow
{
    Q_OBJECT

public:
    explicit DataMonitor(QWidget *parent = nullptr);
    ~DataMonitor();

public slots:
    void getReadedData(QByteArray time, QByteArray data);
    void getSendedData(QByteArray time, QByteArray data);
    void getTimeDiff(qint64 timeDiff);

    void clearTextBrowser();
    void showCurrentTimeoutCounter();
    void showResponse(QByteArray data);
    void showResponseError(QByteArray data);
    void showAverageTime(int currAvrTime);

    void endTest();

private:
    Ui::DataMonitor *ui;
    QFont fontBold;
    int currentTimeout;
    int currentBlock{1};

private slots:
    void increaseFontSize();
    void decreaseFontSize();

    void hideWindow();
    void closeEvent(QCloseEvent *event);

signals:
    void hideMonitor();
    void increaseTimeout();
    void sendNewTime(qint64);
    void requestClearTime();
};

#endif // DATAMONITOR_H
