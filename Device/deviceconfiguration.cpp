#include "deviceconfiguration.h"

static const QByteArray LF = "\010";

DeviceConfiguration::DeviceConfiguration(QObject *parent) : QObject(parent) {

}

QByteArray DeviceConfiguration::setVatRates() {
    return "vatset\tva23,00\tvb8,00\tvc5,00\tvd0,00\tve100\tvf0\tvg100\tda" + QDateTime::currentDateTime().toString("yyyy-MM-dd;HH:mm:ss").toLatin1() + "\t";
}

QByteArray DeviceConfiguration::setHeader() {
    return "hdrset\ttx&cPosnet Polska S.A.&c\t";
}

QByteArray DeviceConfiguration::doDailyRep() {
    return "dailyrep\tda" + QDateTime::currentDateTime().toString("yyyy-MM-dd;HH:mm:ss").toLatin1() + "\t";
}

QByteArray DeviceConfiguration::sfskStatus() {
    return "sfsk\t";
}

QByteArray DeviceConfiguration::scntStatus() {
    return "scnt\t";
}
