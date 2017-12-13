#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QtWidgets>
#include "stdafx.h"
#include "usb.h"
#include <QtConcurrent/QtConcurrent>

class MainWindow : public QWidget
{
    Q_OBJECT
    QListWidget *usb_list;
    QPushButton *btn;
    QGridLayout *grid;
    USB usb;

    static void timer(MainWindow*);
public:
    explicit MainWindow(QWidget *parent = 0);

signals:

public slots:
    void releaseFlash();
    void unmountFlash();
};

#endif // MAINWINDOW_H
