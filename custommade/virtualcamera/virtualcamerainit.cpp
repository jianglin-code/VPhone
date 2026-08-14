#define LOG_TAG "VirtualCamera-INIT"

#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

#include <cutils/log.h>
#include <cutils/properties.h>

//using namespace android;

int main(int /*argc*/, char** /*argv*/)
{
    ALOGI("GuiExt service start...");

	char value1[PROPERTY_VALUE_MAX] = {0};
	property_get("persist.sys.v4l2.camera.back", value1, "");
	char value2[PROPERTY_VALUE_MAX] = {0};
	property_get("persist.sys.v4l2.camera.front", value2, "");

	char value[PROPERTY_VALUE_MAX] = {0};
	property_get("ro.boot.vm.idx", value, "");
	if (strcmp(value, "") != 0) {
		int idx = atoi(value);
		if (idx > 0) {
			if (strcmp(value1, "") == 0) {
				memset(value, 0, PROPERTY_VALUE_MAX);
				sprintf(value, "/dev/video%d", idx*2);
				property_set("persist.sys.v4l2.camera.back", value);
				ALOGD("persist.sys.v4l2.camera.back = %s", value);
			}

			if (strcmp(value2, "") == 0) {
				memset(value, 0, PROPERTY_VALUE_MAX);
				sprintf(value, "/dev/video%d", idx*2+1);
				property_set("persist.sys.v4l2.camera.front", value);
				ALOGD("persist.sys.v4l2.camera.front = %s", value);
			}
		}
	}else{
		if (strcmp(value1, "") == 0) {
			property_set("persist.sys.v4l2.camera.back", "/dev/video0");
		}
		if (strcmp(value2, "") == 0) {
			property_set("persist.sys.v4l2.camera.front", "/dev/video1");
		}
	}

    if (0 == property_get_int32("persist.sys.v4l2.camera.back.w", 0)) {
        property_set("persist.sys.v4l2.camera.back.w", "1920");
    }
    if (0 == property_get_int32("persist.sys.v4l2.camera.back.h", 0)) {
        property_set("persist.sys.v4l2.camera.back.h", "1080");
    }
    if (0 == property_get_int32("persist.sys.v4l2.camera.front.w", 0)) {
        property_set("persist.sys.v4l2.camera.front.w", "1920");
    }
    if (0 == property_get_int32("persist.sys.v4l2.camera.front.h", 0)) {
        property_set("persist.sys.v4l2.camera.front.h", "1080");
    }

    ALOGD("CustomMade service exit...");
    return 0;
}
