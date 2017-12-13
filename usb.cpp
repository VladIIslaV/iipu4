#include "usb.h"
#include <QString>

USB::USB()
{
    usb_list = new char*[10];
    for(int i = 0; i < 10; i++){
        usb_list[i] = new char[128];
    }
    flash = new char[15];
    flash_list = new char*[10];
    for(int i = 0; i < 10; i++){
        flash_list[i] = new char[128];
    }
    prev = new char[4096];
    cur = new char[4096];
}

USB::~USB()
{
    for(int i = 0; i < 10; i++){
        delete(usb_list[i]);
    }
    delete(usb_list);
    delete(flash);
    for(int i = 0; i < 10; i++){
        delete(flash_list[i]);
    }
    delete(flash_list);
    delete(prev);
    delete(cur);
}

int USB::getDeviceList()
{
    int i = 0;
    FILE *cmd = popen("lsusb | grep -v \'hub\\|0ac8\'", "r");
    if(!cmd)
        cout << "Open error" << endl;

    for(i = 0; fgets(usb_list[i], 128, cmd); i++){
        strcpy(usb_list[i], usb_list[i]+23);
        usb_list[i][9] = '\0';
    }
    usb_num = i;
    pclose(cmd);
    if(usb_prev_num == usb_num)
        return -1;
    usb_prev_num = usb_num;
    return i;
}

const char *USB::getDeviceInfo(int num, const char* request, int offset)
{
    char* buffer = new char[100];
    char* command = new char[200];
    command[0] = '\0';
    strcat(command, "lsusb -v -d ");
    strcat(command, usb_list[num]);
    strcat(command, " | grep ");
    strcat(command, request);
    FILE *cmd = popen(command, "r");
    if(!cmd)
        cout << "Open error" << endl;

    fgets(buffer, 100, cmd);
    strcpy(buffer, buffer + offset);
    buffer[strlen(buffer)-1] = '\t';
    pclose(cmd);
    delete(command);
    return buffer;
}

int USB::getFlashList()
{
    char* buffer = new char[100];
    char* tmp = new char[1];
    tmp[0] = (char)10;
    FILE *cmd = popen("cd /media/vladiislav/;dir", "r");
    if(!cmd)
        cout << "Open error" << endl;

    fgets(buffer, 100, cmd);

    int j = 0, i = 0;
    while(true)
    {
        if(buffer[j] == (char)10){
            strcpy(flash_list[i], buffer);
            flash_list[i++][strlen(buffer)-1] = '\0';
            break;
        }
        if(j > 20)
            return NULL;
        j++;
        if(buffer[j-1] == ' ' && buffer[j] == ' '){
            strcpy(flash_list[i], buffer);
            flash_list[i++][j-1] = '\0';
            strcpy(buffer, buffer + j + 1);
        }

    }
    pclose(cmd);
    strcpy(flash, buffer);
    delete(tmp);
    return i;
}

void USB::unmount(const char* fl)
{
    char* command = new char[200];
    command[0] = '\0';
    strcat(command, "umount /media/vladiislav/");
    strcat(command, fl);

    FILE *cmd = popen(command, "r");
    if(!cmd)
        cout << "Open error" << endl;
    delete(command);
    pclose(cmd);
}

bool USB::isReleased()
{
    int i = 0;
    char *tmp = new char[128];
    /*while(i++ < 4096)
        prev[i] = cur[i] = '\0';
*/


    FILE *cmd = popen("lsusb", "r");
    if(!cmd)
        cout << "Open error" << endl;

    for(i = 0; fgets(tmp, 128, cmd); i++)
        strcat(cur, tmp);
        //cout << "34 twrfgv";
    //cout << "34 twrfgv";
    pclose(cmd);
    delete(tmp);

    if(!strstr(cur, prev)){
        return true;
    }
    else{
        strcpy(prev, cur);
        return false;
    }
}
