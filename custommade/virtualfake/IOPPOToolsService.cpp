#define LOG_TAG "Cells/IOPPOToolsService"

#include <utils/Errors.h>
#include <utils/RefBase.h>
#include <utils/Vector.h>
#include <utils/Timers.h>
#include <utils/String16.h>

#include <binder/Parcel.h>
#include <binder/IInterface.h>

#include <cutils/log.h>

#include "IOPPOToolsService.h"

namespace android {

class BpOPPOToolsService : public BpInterface<IOPPOToolsService>
{
public:
    BpOPPOToolsService(const sp<IBinder>& impl) : BpInterface<IOPPOToolsService>(impl)
    {
    }

    virtual status_t test(const String16& name)
    {
        Parcel data, reply;
        data.writeInterfaceToken(IOPPOToolsService::getInterfaceDescriptor());
        data.writeString16(name);
        status_t result = remote()->transact(TEST, data, &reply);
        if (result != NO_ERROR) {
            ALOGE("could not test\n");
            return result;
        }
        result = reply.readInt32();
        return result;
    }
};

IMPLEMENT_META_INTERFACE(OPPOToolsService, "OPPOToolsService")

status_t BnOPPOToolsService::onTransact(uint32_t code, const Parcel& data, Parcel* reply, uint32_t flags)
{
    switch(code)
    {
        case TEST:
        {
            CHECK_INTERFACE(IOPPOToolsService, data, reply);
            String16 name = data.readString16();
            status_t result = test(name);
            reply->writeInt32(result);
            return NO_ERROR;
        }
        break;
    }
    return BBinder::onTransact(code, data, reply, flags);
}

}
