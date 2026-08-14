#define LOG_TAG "Cells/OPPOToolsService"

#include <cutils/log.h>
#include <binder/BinderService.h>
#include <OPPOToolsService.h>

using namespace android;

int main(int /*argc*/, char** /*argv*/)
{
    signal(SIGPIPE, SIG_IGN);

    ALOGI("Service start...");
    OPPOToolsService::instantiate();

    ProcessState::self()->setThreadPoolMaxThreadCount(4);
    ProcessState::self()->startThreadPool();
    IPCThreadState::self()->joinThreadPool();
    ALOGD("Service exit...");
    return 0;
}
