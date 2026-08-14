#ifndef __OPPOCORESERVICE__
#define __OPPOCORESERVICE__

#include "IOPPOCoreService.h"
#include <android-base/properties.h>

namespace android
{

class String16;
class OPPOCoreService :
        public BinderService<OPPOCoreService>,
        public BnOPPOCoreService
{
    friend class BinderService<OPPOCoreService>;
public:

    OPPOCoreService();
    ~OPPOCoreService();

    static char const* getServiceName() {
        static std::string mServiceName = android::base::GetProperty("ro.custommade.deviceinfo.servicename1", "OPPOCoreService");
        return mServiceName.c_str();
    }

    virtual status_t test(const String16& name);

};
};
#endif
