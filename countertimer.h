#ifndef COUNTERTIMER_H
#define COUNTERTIMER_H

#include <QObject>
#include <QList>
#include <QApplication>
#include <QDateTime>

class CounterTimer : public QObject
{
    Q_OBJECT
public:
    explicit CounterTimer(QObject *parent = nullptr);

    void countTime(qint64 newTime);
    void clearTimes();

private:
    QList<int> listTime;
    int sumTime{0};

signals:
    void sendCurrentAverageTime(int);
};

#endif // COUNTERTIMER_H
