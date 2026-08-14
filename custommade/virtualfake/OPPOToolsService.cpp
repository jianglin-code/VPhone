#define LOG_TAG "Cells/OPPOToolsService"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>

#include <cutils/log.h>
#include <cutils/properties.h>
#include "OPPOToolsService.h"

namespace android {

OPPOToolsService::OPPOToolsService()
{
}

OPPOToolsService::~OPPOToolsService()
{
}

status_t OPPOToolsService::test(const String16& name)
{
    return NO_ERROR;
}

};
