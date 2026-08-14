
#ifndef __CELLSSERVICE_UTILS_H__
#define __CELLSSERVICE_UTILS_H__

#include <utils/String16.h>

void* tar_pthead(void* o);
void* untar_pthead(void* o);
void* download_pthead(void* o);
void* upload_pthead(void* o);

void write_vm_exit(bool bexit);
void create_gotosleep_pthread(void);
int sendBroadcastMessage(const char* package, const char* action, int value);

void* gotosleep(void* o);
void* gotoreboot(void* o);
void* gotoshutdown(void* o);
void* gotostart(void* o);

android::String16 GetVMIP(const char* interface);
android::String16 GetRouterVMIP(const char* interface);
void StartAdbProxy(const char* name);

#endif // __CELLSSERVICE_UTILS_H__