#ifndef DATAMONITOR_H
#define DATAMONITOR_H

#include <QMainWindow>
#include <QFont>
#include <QTextCursor>
#include <QCloseEvent>
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

    void showResponse(QByteArray data);
    void showResponseError(QByteArray data);

private:
    Ui::DataMonitor *ui;
    QFont fontBold;

private slots:
    void increaseFontSize();
    void decreaseFontSize();

    void hideWindow();
    void closeEvent(QCloseEvent *event);

signals:
    void hideMonitor();
};

#endif // DATAMONITOR_H
