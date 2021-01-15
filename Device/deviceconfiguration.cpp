#include "deviceconfiguration.h"

DeviceConfiguration::DeviceConfiguration(QObject *parent) : QObject(parent) {

}

QByteArray DeviceConfiguration::setVatRates() {
    return "vatset\tva23,00\tvb8,00\tvc5,00\tvd0,00\tve100\tvf0\tvg100\tda" + QDateTime::currentDateTime().toString("yyyy-MM-dd;HH:mm:ss").toLatin1() + "\t";
}

QByteArray DeviceConfiguration::setHeader() {
    return "hdrset\ttx&1&cPosnet Polska S.A.&c&1\0A&2&c02-281&2&3 Warszawa&3\0A&5&cMunicypalna&5&6 33&6\tpr1\t";
}

QByteArray DeviceConfiguration::doDailyRep() {
    return "dailyrep\tda" + QDateTime::currentDateTime().toString("yyyy-MM-dd;HH:mm:ss").toLatin1() + "\t";
}

QByteArray DeviceConfiguration::sfskStatus() {
    return "sfsk\t";
}
