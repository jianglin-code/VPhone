#define LOG_TAG "Cells/cellsfunctiontoolservice"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>

#include <selinux/android.h>

#include <cutils/log.h>
#include <cutils/properties.h>
#include "CellsFunctionToolService.h"

namespace android {

CellsFunctionToolService::CellsFunctionToolService()
{
}

CellsFunctionToolService::~CellsFunctionToolService()
{
}

status_t CellsFunctionToolService::setProperty(const String16& name,const String16& value)
{
    ALOGD("SETPROPERTY arg %s %s", String8(name).string(), String8(value).string());
    status_t result = property_set(String8(name).string(), String8(value).string());
    ALOGD("SETPROPERTY result = %d", result);
    return result;
}

status_t CellsFunctionToolService::getProperty(const String16& name,String16& value)
{
    char v[PROPERTY_VALUE_MAX] = {0};
    property_get(String8(name).string(), v, "");
    value = android::String16(v);
    return NO_ERROR;
}

int CellsFunctionToolService::getPidNamespace(const int pid, int* hostpid, int* cellpid)
{
    FILE *fp;
    char buffer[64] = {0};
    char path[64] = {0};
    int ret = 0;

    snprintf(path, sizeof(path), "/proc/%d/drv_name", pid);
    fp = fopen(path, "r");
    if (fp == NULL) {
        ALOGW("failed to open %s", path);
        return -1;
    }

    if (fgets(buffer, 64, fp) != NULL) {
        sscanf(buffer, "cell%d", &ret); 
    }

    if(hostpid != NULL){
        memset(buffer, 0, 64);
        if (fgets(buffer, 64, fp) != NULL) {
            *hostpid = std::atoi(buffer);
        }
    }

    if(cellpid != NULL){
        memset(buffer, 0, 64);
        if (fgets(buffer, 64, fp) != NULL) {
            *cellpid = std::atoi(buffer);
        }
    }

    if (property_get_int32("ro.boot.vm.idx", 0) == ret) {
        ret = -1;
    }

    //ALOGD("getPidNamespace ret=%d hostpid=%d cellpid=%d \n", ret, *hostpid, *cellpid);
    fclose(fp);
    return ret;
}

status_t CellsFunctionToolService::setEnforce(const int enforce)
{
    status_t ret = -1;
    if (enforce == 1 || enforce == 0){
        ret = security_setenforce(enforce);
    }
    return ret;
}

status_t CellsFunctionToolService::isEnforce(int* enforce)
{
    if (enforce){
        *enforce = security_getenforce();
    }
    return NO_ERROR;
}

};
