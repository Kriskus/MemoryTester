#include "countertimer.h"

CounterTimer::CounterTimer(QObject *parent) : QObject(parent) {

}

void CounterTimer::countTime(qint64 newTime) {
    qApp->processEvents();
    if(newTime != 0)
        listTime.append(newTime);
    sumTime = 0;
    foreach (int time, listTime) {
        sumTime += time;
    }
    emit sendCurrentAverageTime(sumTime/listTime.size());
}

void CounterTimer::clearTimes() {
    qApp->processEvents();
    listTime.clear();
    sumTime = 0;
}
