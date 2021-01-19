#ifndef PREPARINGDEVICEWINDOW_H
#define PREPARINGDEVICEWINDOW_H

#include <QMainWindow>
#include <QDateTime>
#include <QCloseEvent>

#include "Other/countercrc.h"
#include "Device/deviceconfiguration.h"

namespace Ui {
class PreparingDeviceWindow;
}

class PreparingDeviceWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit PreparingDeviceWindow(QWidget *parent = nullptr, bool currentStatusConnection = false);
    ~PreparingDeviceWindow();

public slots:
    void getStatusConnection(bool currentStatus);
    void setDeviceInformation(QByteArray data);

    void setHeader();
    void setVatRates();
    void doDailyReport();

private:
    Ui::PreparingDeviceWindow *ui;
    CounterCrc *countCrc;
    DeviceConfiguration *confDevice;

    bool statusConnection{false};

private slots:
    void hideWindow();
    void closeEvent(QCloseEvent *event);

signals:
    void sendSequenceToDevice(QByteArray);
    void hideMonitor();
};

#endif // PREPARINGDEVICEWINDOW_H
