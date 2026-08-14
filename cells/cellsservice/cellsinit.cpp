#define LOG_TAG "Cells/cellsinit"

#include <fcntl.h>
#include <string.h>

#include <cutils/log.h>
#include <cutils/properties.h>

int main(int /*argc*/, char** /*argv*/)
{
	int fdname = open("/.name",O_RDONLY);
	if (fdname >= 0) {
		char value[PROPERTY_VALUE_MAX] = {0};
		int len = read(fdname, value, PROPERTY_VALUE_MAX);
		if (len > 0) {
			property_set("ro.boot.vm.name", value);
			ALOGD("ro.boot.vm.name = %s", value);

			int index = 0;
			sscanf(value, "cell%d", &index);
			memset(value, 0, PROPERTY_VALUE_MAX);
			sprintf(value, "%d", index);
			property_set("ro.boot.vm.idx", value);
			ALOGD("ro.boot.vm.idx = %s", value);
		}
		close(fdname);
		fdname = 0;
	}
}
