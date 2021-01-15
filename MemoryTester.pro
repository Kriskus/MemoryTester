QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets serialport network

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Device/deviceconfiguration.cpp \
    Device/devicedatabase.cpp \
    Device/devicetransaction.cpp \
    Device/testeroperation.cpp \
    Logs/logdata.cpp \
    datamonitor.cpp \
    main.cpp \
    mainwindow.cpp \
    preparingdevicewindow.cpp \
    testerwindow.cpp

HEADERS += \
    Device/deviceconfiguration.h \
    Device/devicedatabase.h \
    Device/devicetransaction.h \
    Device/testeroperation.h \
    Logs/logdata.h \
    Other/countercrc.h \
    datamonitor.h \
    mainwindow.h \
    preparingdevicewindow.h \
    testerwindow.h

FORMS += \
    datamonitor.ui \
    mainwindow.ui \
    preparingdevicewindow.ui \
    testerwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
