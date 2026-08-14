#ifndef __IOPPOTOOLSSERVICE__
#define __IOPPOTOOLSSERVICE__

#include <binder/IInterface.h>
#include <binder/Parcel.h>
#include <binder/BinderService.h>
#include <cutils/properties.h>
#include <utils/String16.h>

namespace android
{

class String16;
class IOPPOToolsService : public IInterface
{
protected:
    enum {
        TEST = IBinder::FIRST_CALL_TRANSACTION,
    };

public:
    DECLARE_META_INTERFACE(OPPOToolsService)

    virtual status_t test(const String16& name) = 0;
};

class BnOPPOToolsService : public BnInterface<IOPPOToolsService>
{
    virtual status_t onTransact(uint32_t code,
                                const Parcel& data,
                                Parcel* reply,
                                uint32_t flags = 0);
};

}

#endif
