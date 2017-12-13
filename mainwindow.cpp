#include "mainwindow.h"

void MainWindow::timer(MainWindow *parent)
{
    while(true){
        emit(parent->releaseFlash());
        usleep(100000);
    }
}

MainWindow::MainWindow(QWidget *parent) : QWidget(parent)
{
    this->setFixedSize(500, 300);
    usb_list = new QListWidget(this);

    btn = new QPushButton(this);
    btn->setText("Unmount");
    connect(btn, SIGNAL(clicked()), this, SLOT(unmountFlash()));

    grid = new QGridLayout(this);
    grid->addWidget(usb_list, 0, 0);
    grid->addWidget(btn, 1, 0);

    QtConcurrent::run(timer, this);
}

void MainWindow::releaseFlash()
{
    int i = 0;
    char* buf = new char[100];

    i = usb.getDeviceList();
    if(i == -1)
        return;
    usb_list->clear();

    while(i--){
        buf[0] = '\0';
        if(strcmp(usb.getDeviceInfo(i, "iInterface", 30), "5") >= 0){
            strcat(buf, usb.getDeviceInfo(i, "Manufacturer", 28));
            strcat(buf, usb.getDeviceInfo(i, "iProduct", 28));
            usb_list->addItem(buf);
        }
        else{
            if(strcmp(usb.getDeviceInfo(i, "bInterfaceClass", 30), "8") < 0){
                strcat(buf, usb.getDeviceInfo(i, "idVendor", 28));
                strcat(buf, usb.getDeviceInfo(i, "idProduct", 28));
                usb_list->addItem(buf);
            }
        }
    }
    i = usb.getFlashList();
    while(i--)
        usb_list->addItem(usb.flash_list[i]);
    delete(buf);
}

void MainWindow::unmountFlash()
{
    if(usb_list->currentItem() != NULL)
        usb.unmount(usb_list->currentItem()->text().toStdString().c_str());
}
