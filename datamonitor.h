#ifndef DATAMONITOR_H
#define DATAMONITOR_H

#include <QMainWindow>
#include <QFont>
#include <QTextCursor>
#include <QCloseEvent>
#include <QDateTime>
#include <QThread>

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

private:
    Ui::DataMonitor *ui;
    QFont fontBold;
    int currentTimeout;

private slots:
    void increaseFontSize();
    void decreaseFontSize();

    void hideWindow();
    void closeEvent(QCloseEvent *event);

signals:
    void hideMonitor();
    void increaseTimeout();
};

#endif // DATAMONITOR_H
