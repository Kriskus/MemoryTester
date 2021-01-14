#ifndef DEVICEDATABASE_H
#define DEVICEDATABASE_H

#include <QObject>

class DeviceDataBase : public QObject
{
    Q_OBJECT
public:
    explicit DeviceDataBase(QObject *parent = nullptr);

public slots:
    QByteArray dbCheckTrLine(int currentVat, int currentPosition);
    QByteArray dbCheckFvLine(int currentVat, int currentPosition);
};

#endif // DEVICEDATABASE_H
