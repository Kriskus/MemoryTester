#include "mainwindow.h"

#include <QApplication>
#include <QDir>

int main(int argc, char *argv[]) {
    if(!QDir("Logs").exists())
        QDir().mkdir("Logs");
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
