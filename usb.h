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

    char **usb_list, *flash, **flash_list;
    int usb_num;
};

#endif // USB_H
