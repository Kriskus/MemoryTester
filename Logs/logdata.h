#ifndef LOGDATA_H
#define LOGDATA_H

#include <QObject>
#include <QDateTime>
#include <QFile>

class LogData : public QObject
{
    Q_OBJECT
public:
    explicit LogData(QObject *parent = nullptr, QString fileLogName = QString());

public slots:
    void appendLogFile(QByteArray time, QByteArray data);
    void convertToQByteArray(qint64 timeDiff);

private:
    QString fileName;
    bool saveError{false};
    int currentBlock{1};

signals:
    void sendMessageBoxInformation(QString);
    void finished();
};

#endif // LOGDATA_H
