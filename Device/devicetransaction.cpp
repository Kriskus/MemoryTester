#include "devicetransaction.h"

DeviceTransaction::DeviceTransaction(QObject *parent) : QObject(parent) {

}

QByteArray DeviceTransaction::invoiceInit() {
    return "trfvinit\t";
}

QByteArray DeviceTransaction::invoiceBuyer() {
    return "trfvbuyer\tnaFirma\tKrzaq\tniNIP 113-114-16-15\tsc0\t";
}

QByteArray DeviceTransaction::invoiceNumber() {
    return "trfvnumber\tnbFaktura_Numer_Raz\tsc0\t";
}

QByteArray DeviceTransaction::transactionInit() {
    return "trinit\t";
}

QByteArray DeviceTransaction::transactionTrEnd() {
    return "trend\tto50000\t";
}

QByteArray DeviceTransaction::transactionFvEnd() {
    return "trend\tto12000\t";
}

QByteArray DeviceTransaction::transactionTrLine(int positionNumber, int vatRate) {
    return "trline\tnaTowar PR" + QByteArray::number(positionNumber) + "\tpr100\tvt" + QByteArray::number(vatRate) + "\t";
}

QByteArray DeviceTransaction::transactionInLine(int positionNumber, int vatRate) {
    return "trline\tnaTowar FV" + QByteArray::number(positionNumber) + "\tpr100\tvt" + QByteArray::number(vatRate) + "\t";
}
