#define LOG_TAG "Cells/OPPOCoreService"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>

#include <cutils/log.h>
#include <cutils/properties.h>
#include "OPPOCoreService.h"

namespace android {

OPPOCoreService::OPPOCoreService()
{
}

OPPOCoreService::~OPPOCoreService()
{
}

status_t OPPOCoreService::test(const String16& name)
{
    return NO_ERROR;
}

};
