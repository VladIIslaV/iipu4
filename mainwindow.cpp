#include "mainwindow.h"

void MainWindow::timer(MainWindow *parent)
{
    while(true){
        emit(parent->releaseFlash());
        sleep(5);
    }
}

void MainWindow::chooseDevice(QListWidgetItem* it)
{
    cout << "@#$@#$@#$@#$@#$";
    strcpy(device, it->text().toStdString().c_str());
}

MainWindow::MainWindow(QWidget *parent) : QWidget(parent)
{
    this->setFixedSize(500, 300);
    usb_list = new QListWidget(this);
    connect(usb_list,
            SIGNAL(activated(QModelIndex)),
            this,
            SLOT(chooseDevice(QListWidgetItem*))
            );

    btn = new QPushButton(this);
    btn->setText("Unmount");
    connect(btn, SIGNAL(clicked()), this, SLOT(unmountFlash()));

    grid = new QGridLayout(this);
    grid->addWidget(usb_list, 0, 0);
    grid->addWidget(btn, 1, 0);

    QtConcurrent::run(timer, this);
    device = new char[15];
}

void MainWindow::releaseFlash()
{
    int i = 0;
    char* buf = new char[100];
    while (i--)
        buf[i] = '\0';

    usb_list->clear();

    i = usb.getDeviceList();
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
    while(i--){
        usb_list->addItem(usb.flash_list[i]);
    }
    delete(buf);
}

void MainWindow::unmountFlash()
{
    usb.unmount(usb_list->currentItem()->text().toStdString().c_str());
}
