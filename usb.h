#ifndef USB_H
#define USB_H

#include "stdafx.h"

struct Flash{
    char* name;
    char* path;
    char* size;
};

class USB
{

public:
    USB();
    ~USB();
    int getDeviceList();
    const char* getDeviceInfo(int, const char*, int);
    int getFlashList();
    void unmount(const char*);
    bool isReleased();

    char **usb_list, *flash, **flash_list;
    char *prev, *cur;
    int usb_num, usb_prev_num;
};

#endif // USB_H
