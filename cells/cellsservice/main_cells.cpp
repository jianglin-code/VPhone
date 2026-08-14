#define LOG_TAG "Cells/cellsservice"

#include <cutils/log.h>
#include <binder/BinderService.h>
#include <CellsFunctionToolService.h>
#include <CellsPrivateService.h>

using namespace android;

int main(int /*argc*/, char** /*argv*/)
{
    signal(SIGPIPE, SIG_IGN);

    ALOGI("Service start...");
    CellsFunctionToolService::instantiate();
    CellsPrivateService::instantiate();

    ProcessState::self()->setThreadPoolMaxThreadCount(4);
    ProcessState::self()->startThreadPool();
    IPCThreadState::self()->joinThreadPool();
    ALOGD("Service exit...");
    return 0;
}
