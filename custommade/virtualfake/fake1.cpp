#define LOG_TAG "Cells/OPPOCoreService"

#include <cutils/log.h>
#include <binder/BinderService.h>
#include <OPPOCoreService.h>

using namespace android;

int main(int /*argc*/, char** /*argv*/)
{
    signal(SIGPIPE, SIG_IGN);

    ALOGI("Service start...");
    OPPOCoreService::instantiate();

    ProcessState::self()->setThreadPoolMaxThreadCount(4);
    ProcessState::self()->startThreadPool();
    IPCThreadState::self()->joinThreadPool();
    ALOGD("Service exit...");
    return 0;
}
