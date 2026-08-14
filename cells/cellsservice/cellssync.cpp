#define LOG_TAG "Cells/cellssync"

#include <cutils/log.h>
#include <binder/BinderService.h>
#include "ICellsPrivateService.h"
#include "ICellsFunctionToolService.h"

using namespace android;

static void switchtohost()
{
    const sp<IServiceManager> sm = defaultServiceManager();
    if (sm != NULL) {
        sp<IBinder> binder = sm->checkService(String16("CellsPrivateService"));
        if (binder != NULL) {
            sp<ICellsPrivateService> pCellsPrivateService = interface_cast<ICellsPrivateService>(binder);
            if(pCellsPrivateService == NULL){
                ALOGE("could not get service CellsPrivateService \n");
                return;
            }

            pCellsPrivateService->switchCellsVM(android::String16("host"));
        }
    }
}

int main(int argc, char** argv)
{
    char vm[PROPERTY_VALUE_MAX] = {0};
    property_get("ro.boot.vm", vm, "0");
    if (strcmp(vm, "1") != 0) {
        return 0;
    }

    const sp<IServiceManager> sm = initdefaultServiceManager();
    if (sm == NULL) {
        return 0;
    }

    ALOGD("argc = %d ", argc);
    if(argc == 2 && strcmp(argv[1],"sys.powerctl") == 0)
    {
        char value[PROPERTY_VALUE_MAX];
        property_get(argv[1], value, "");

        ALOGD("argv[1] = %s value = %s", argv[1], value);
        if(strstr(value,"reboot") != NULL)
        {
            char name[PROPERTY_VALUE_MAX] = {0};
            property_get("ro.boot.vm.name", name, "");

            sp<ICellsPrivateService> pCellsPrivateService = 
                    interface_cast<ICellsPrivateService>(sm->checkService(String16("CellsPrivateService")));
            if(pCellsPrivateService != NULL){
                switchtohost();
                sleep(3);
                pCellsPrivateService->rebootVM(android::String16(name));
            }
            return 0;
        }

        if(strstr(value,"shutdown") != NULL)
        {
            char name[PROPERTY_VALUE_MAX] = {0};
            property_get("ro.boot.vm.name", name, "");

            sp<ICellsPrivateService> pCellsPrivateService = 
                    interface_cast<ICellsPrivateService>(sm->checkService(String16("CellsPrivateService")));
            if(pCellsPrivateService != NULL){
                switchtohost();
                sleep(3);
                pCellsPrivateService->shutdownVM(android::String16(name));
            }
            return 0;
        }             
    }

    if (argc == 2)
    {
        char value[PROPERTY_VALUE_MAX];
        property_get(argv[1], value, "");
        sp<ICellsFunctionToolService> pICellsFunctionToolService = 
                interface_cast<ICellsFunctionToolService>(sm->checkService(String16("CellsFunctionToolService")));
        if(pICellsFunctionToolService != NULL){
            pICellsFunctionToolService->setProperty(android::String16(argv[1]),android::String16(value));
        }
    }
    else if (argc == 3) 
    {
        sp<ICellsFunctionToolService> pICellsFunctionToolService = 
                interface_cast<ICellsFunctionToolService>(sm->checkService(String16("CellsFunctionToolService")));
        if(pICellsFunctionToolService != NULL){
            pICellsFunctionToolService->setProperty(android::String16(argv[1]),android::String16(argv[2]));
        }
    }

    return 0;
}
