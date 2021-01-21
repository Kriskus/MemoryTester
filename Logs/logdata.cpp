#include "logdata.h"

LogData::LogData(QObject *parent, QString fileLogName) : QObject(parent), fileName(fileLogName) {
    appendLogFile("#" + QByteArray::number(currentBlock++), "");
}

void LogData::appendLogFile(QByteArray time, QByteArray data) {
    if(!saveError) {
        QFile logFile(fileName);
        if(logFile.open(QIODevice::Append)) {
            logFile.write(time + "\n" + data + "\n");
            logFile.close();
        } else {
            emit sendMessageBoxInformation("Błąd zapisu pliku logowania monitora transakcji\n\n" + logFile.errorString());
            saveError = true;
        }
    } else {
        emit finished();
    }

}

void LogData::convertToQByteArray(qint64 timeDiff) {
    appendLogFile("Różnica ", QByteArray::number(timeDiff) + "ms\n---------------------------\n#" + QByteArray::number(currentBlock++));
}

