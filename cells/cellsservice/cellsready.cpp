#define LOG_TAG "Cells/cellsready"

#include <cutils/log.h>
#include <binder/BinderService.h>
#include "ICellsPrivateService.h"

using namespace android;

int main(int /*argc*/, char** /*argv*/)
{
    const sp<IServiceManager> sm = initdefaultServiceManager();
    if (sm != NULL) {
        sp<IBinder> binder = sm->checkService(String16("CellsPrivateService"));
        if (binder != NULL) {
            sp<ICellsPrivateService> pCellsPrivateService = interface_cast<ICellsPrivateService>(binder);
            if(pCellsPrivateService == NULL){
                ALOGE("could not get service CellsPrivateService \n");
                return 0;
            }

            char name[PROPERTY_VALUE_MAX] = {0};
            property_get("ro.boot.vm.name", name, "");
            pCellsPrivateService->vmSystemReady(android::String16(name));
        }
    }

    return 0;
}
