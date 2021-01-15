#include "preparingdevicewindow.h"
#include "ui_preparingdevicewindow.h"

PreparingDeviceWindow::PreparingDeviceWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PreparingDeviceWindow)
{
    ui->setupUi(this);
}

PreparingDeviceWindow::~PreparingDeviceWindow()
{
    delete ui;
}
