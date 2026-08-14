#ifndef __OPPOTOOLSSERVICE__
#define __OPPOTOOLSSERVICE__

#include "IOPPOToolsService.h"
#include <android-base/properties.h>

namespace android
{

class String16;
class OPPOToolsService :
        public BinderService<OPPOToolsService>,
        public BnOPPOToolsService
{
    friend class BinderService<OPPOToolsService>;
public:

    OPPOToolsService();
    ~OPPOToolsService();

    static char const* getServiceName() {
        static std::string mServiceName = android::base::GetProperty("ro.custommade.deviceinfo.servicename2", "OPPOToolsService");
        return mServiceName.c_str();
    }

    virtual status_t test(const String16& name);

};
};
#endif
