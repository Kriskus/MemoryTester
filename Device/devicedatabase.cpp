#include "devicedatabase.h"

DeviceDataBase::DeviceDataBase(QObject *parent) : QObject(parent) {

}

QByteArray DeviceDataBase::dbCheckTrLine(int currentVat, int currentPosition) {
    return "dbchkplu\tvt" + QByteArray::number(currentVat) + "\tnaTowar PR" + QByteArray::number(currentPosition) + "\t";
}

QByteArray DeviceDataBase::dbCheckFvLine(int currentVat, int currentPosition) {
    return "dbchkplu\tvt" + QByteArray::number(currentVat) + "\tnaTowar FV" + QByteArray::number(currentPosition) + "\t";
}
