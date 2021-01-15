#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDateTime>
#include <QTcpSocket>
#include <QMessageBox>
#include <QCloseEvent>
#include <QThread>
#include <QTimer>

#include "datamonitor.h"
#include "testerwindow.h"
#include "Logs/logdata.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void deviceConnect();
    void deviceDisconnect();
    void deviceReadWrite(QByteArray data);
    void serialConnect();
    void serialDisconnect();
    void serialReadWrite(QByteArray data);
    void socketConnect();
    void socketDisconnect();
    void socketReadWrite(QByteArray data);
    void showAvailableDevices();
    void showMessageBoxInformation(QString information);
    void showStatusBarInformation(QString information);

    void prepareDataMonitorWindow();
    void showDataMonitorWindow();
    void changeDataMonitorWindowStatus();
    void prepareTesterWindow();
    void showTesterWindow();
    void changeTesterWindowStatus();

    void logThread();
    void showVersionInformation();

    void closeWindow();
    void closeEvent(QCloseEvent *event);

private:
    Ui::MainWindow *ui;
    QSerialPort *serial;
    QTcpSocket *socket;
    TesterWindow *testerMonitor;
    DataMonitor *dataMonitor;
    QTimer *timerResponse;

    QDateTime timeWrite;
    QDateTime timeRead;

    bool connectionStatus{false};
    bool dataMonitorWindowStatus{false};
    bool testerWindowStatus{false};
    QByteArray response;

signals:
    void finished();
    void sendReadedData(QByteArray, QByteArray);
    void sendWritedData(QByteArray, QByteArray);
    void sendTimeDifference(qint64);
    void sendStatusBarInformation(QString);
    void sendMessageBoxInformation(QString);
    void sendConnectionStatusDevice(bool);

};
#endif // MAINWINDOW_H
