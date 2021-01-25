#ifndef TESTERWINDOW_H
#define TESTERWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include <QThread>

#include "Device/testeroperation.h"

namespace Ui {
class TesterWindow;
}

class TesterWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit TesterWindow(QWidget *parent = nullptr, bool currentStatus = false);
    ~TesterWindow();

public slots:
    void getConnectionStatus(bool currentStatus);

private:
    Ui::TesterWindow *ui;
    bool statusConnection{false};
    bool threadInProgress{false};

private slots:
    void getSequenceToSend(QByteArray data);
    void getLastNumbers(int trNum, int fvNum);

    void startTestingThread();
    void stopTestingThread();
    void showCurrentRepeat(int currentRepeat);
    void setCustom(int);

    void hideWindow();
    void closeEvent(QCloseEvent *event);

signals:
    void hideMonitor();
    void sendSequenceToDevice(QByteArray);
    void sendMessageBoxInformation(QString);
    void endTest();
    void stopThread();
};

#endif // TESTERWINDOW_H
