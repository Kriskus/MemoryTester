#ifndef PREPARINGDEVICEWINDOW_H
#define PREPARINGDEVICEWINDOW_H

#include <QMainWindow>

namespace Ui {
class PreparingDeviceWindow;
}

class PreparingDeviceWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit PreparingDeviceWindow(QWidget *parent = nullptr);
    ~PreparingDeviceWindow();

private:
    Ui::PreparingDeviceWindow *ui;
};

#endif // PREPARINGDEVICEWINDOW_H
