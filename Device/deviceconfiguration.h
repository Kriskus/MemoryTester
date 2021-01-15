#ifndef DEVICECONFIGURATION_H
#define DEVICECONFIGURATION_H

#include <QObject>
#include <QDateTime>

class DeviceConfiguration : public QObject
{
    Q_OBJECT
public:
    explicit DeviceConfiguration(QObject *parent = nullptr);

public slots:
    QByteArray setVatRates();
    QByteArray setHeader();
    QByteArray doDailyRep();
    QByteArray sfskStatus();

signals:
    void sendSequence(QByteArray);
};

#endif // DEVICECONFIGURATION_H
