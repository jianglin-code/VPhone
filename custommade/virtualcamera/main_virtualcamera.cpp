#define LOG_TAG "VirtualCameraService"

#include <fcntl.h>

#include <cutils/log.h>
#include <cutils/properties.h>
#include <binder/BinderService.h>
#include <android/native_window.h>

#include <VirtualCameraService.h>

using namespace android;

int main(int /*argc*/, char** /*argv*/)
{
	signal(SIGPIPE, SIG_IGN);

	ALOGI("Service start...");
	ProcessState::self()->setThreadPoolMaxThreadCount(4);
	VirtualCameraService::publishAndJoinThreadPool(true);
	ALOGD("Service exit...");
    return 0;
}
