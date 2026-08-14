#define LOG_TAG "CellsBase"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>

#include <cutils/log.h>
#include <android-base/properties.h>

namespace android {

bool getinithidlservice(const char* descriptor)
{
    static const char* inithidlservice[]={
        "android.hardware.bluetooth",
        "android.hardware.nfc",
        "android.hardware.oemlock",
        "android.hardware.weaver",
        "android.hardware.authsecret",
        "android.hardware.sensors",
        "vendor.qti.data.factory",
        "android.hardware.gatekeeper",
        "android.hardware.keymaster",
        "android.hardware.thermal",
        "android.hardware.audio",
        "android.hardware.soundtrigger",
        "android.hardware.secure_element",
        "android.hardware.radio",
        "vendor.samsung_slsi.telephony.hardware.radioExternal",
        "vendor.google.whitechapel.audio.audioext",
        NULL,
    };

    std::string prop = android::base::GetProperty("ro.boot.vm", "1");
    if (prop == "0") {
        return false;
    }

    //ALOGD("getinithidlservice begin: %s", descriptor);

    for(int i =0;  inithidlservice[i] != NULL;  i++){
        if(strncmp(descriptor, inithidlservice[i], strlen( inithidlservice[i])) == 0){
            ALOGD("getinithidlservice %s", descriptor);
            return true;
        }
    }
    return false;
}

bool getinitaidlservice(const char* name)
{
    static const char* names[]={
        "android.hardware.citadel",
        "android.hardware.oemlock",
        "android.hardware.authsecret",
        "android.hardware.radio",
        "android.hardware.weaver",
        "android.hardware.gnss",
        "android.hardware.biometrics.face",
        "android.hardware.biometrics.fingerprint", //second start could be died.
        "android.hardware.security.sharedsecret",
        "android.hardware.security.keymint",
        "android.hardware.vibrator",
        "android.hardware.composer",
        "android.hardware.graphics.composer3",
        "com.google.hardware.pixel.display",
        "com.google.edgetpu.IEdgeTpuVendorService",
        "android.hardware.usb.gadget.IUsbGadget",
        NULL,
    };

    std::string prop = android::base::GetProperty("ro.boot.vm", "1");
    if (prop == "0") {
        return false;
    }

    for(int i =0;  names[i] != NULL;  i++){
        if(strncmp(name, names[i], strlen(names[i])) == 0){
            ALOGD("getinitaidlservice %s", name);
            return true;
        }
    }
    return false;
}

int getPidNamespace(const int pid, int* hostpid, int* cellpid)
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

    if (android::base::GetIntProperty("ro.boot.vm.idx", 0) == ret) {
        ret = -1;
    }

    //ALOGD("getPidNamespace ret=%d hostpid=%d cellpid=%d \n", ret, *hostpid, *cellpid);
    fclose(fp);
    return ret;
}

} // namespace cells
