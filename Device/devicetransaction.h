#ifndef DEVICETRANSACTION_H
#define DEVICETRANSACTION_H

#include <QObject>

class DeviceTransaction : public QObject
{
    Q_OBJECT
public:
    explicit DeviceTransaction(QObject *parent = nullptr);

public slots:
    QByteArray invoiceInit();
    QByteArray invoiceBuyer();
    QByteArray invoiceNumber();
    QByteArray transactionInit();
    QByteArray transactionTrEnd();
    QByteArray transactionFvEnd();
    QByteArray transactionTrLine(int positionNumber, int vatRate);
    QByteArray transactionInLine(int positionNumber, int vatRate);

signals:

};

#endif // DEVICETRANSACTION_H
