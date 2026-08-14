#define LOG_TAG "Cells/ICellsFunctionToolService"

#include <utils/Errors.h>
#include <utils/RefBase.h>
#include <utils/Vector.h>
#include <utils/Timers.h>
#include <utils/String16.h>

#include <binder/Parcel.h>
#include <binder/IInterface.h>

#include <cutils/log.h>

#include "ICellsFunctionToolService.h"

namespace android {

class BpCellsFunctionToolService : public BpInterface<ICellsFunctionToolService>
{
public:
    BpCellsFunctionToolService(const sp<IBinder>& impl) : BpInterface<ICellsFunctionToolService>(impl)
    {
    }

    virtual status_t setProperty(const String16& name,const String16& value)
    {
        Parcel data, reply;
        data.writeInterfaceToken(ICellsFunctionToolService::getInterfaceDescriptor());
        data.writeString16(name);
        data.writeString16(value);
        status_t result = remote()->transact(SETPROPERTY, data, &reply);
        if (result != NO_ERROR) {
            ALOGE("could not set property");
            return result;
        }
        result = reply.readInt32();
        return result;
    }

    virtual status_t getProperty(const String16& name,String16& value)
    {
        Parcel data, reply;
        data.writeInterfaceToken(ICellsFunctionToolService::getInterfaceDescriptor());
        data.writeString16(name);
        status_t result = remote()->transact(GETPROPERTY, data, &reply);
        if (result != NO_ERROR) {
            ALOGE("could not get property");
            return result;
        }
        value = reply.readString16();
        result = reply.readInt32();
        return result;
    }

    virtual int getPidNamespace(const int pid, int* hostpid, int* cellpid)
    {
        Parcel data, reply;
        data.writeInterfaceToken(ICellsFunctionToolService::getInterfaceDescriptor());
        data.writeInt32(pid);
        status_t result = remote()->transact(PIDNAMESPACE, data, &reply);
        if (result != NO_ERROR) {
            ALOGE("could not getPidNamespace");
            return 0;
        }

        int cpid = reply.readInt32();
        int hpid = reply.readInt32();
        if (cellpid != NULL) *cellpid = cpid;
        if (hostpid != NULL) *hostpid = hpid;
        return reply.readInt32();
    }

    virtual status_t setEnforce(const int enforce)
    {
        Parcel data, reply;
        data.writeInterfaceToken(ICellsFunctionToolService::getInterfaceDescriptor());
        data.writeInt32(enforce);
        status_t result = remote()->transact(SETENFORCE, data, &reply);
        if (result != NO_ERROR) {
            ALOGE("could not set enforce");
            return result;
        }
        result = reply.readInt32();
        return result;
    }

    virtual status_t isEnforce(int* enforce)
    {
        Parcel data, reply;
        data.writeInterfaceToken(ICellsFunctionToolService::getInterfaceDescriptor());
        status_t result = remote()->transact(ISENFORCE, data, &reply);
        if (result != NO_ERROR) {
            ALOGE("could not get enforce");
            return 0;
        }

        int v = reply.readInt32();
        if (enforce != NULL) *enforce = v;
        return reply.readInt32();
    }

};

IMPLEMENT_META_INTERFACE(CellsFunctionToolService, "CellsFunctionToolService")

status_t BnCellsFunctionToolService::onTransact(uint32_t code, const Parcel& data, Parcel* reply, uint32_t flags)
{
    switch(code)
    {
        case SETPROPERTY:
        {
            CHECK_INTERFACE(ICellsFunctionToolService, data, reply);
            String16 name = data.readString16();
            String16 value = data.readString16();

            status_t result = setProperty(name, value);
            reply->writeInt32(result);
            return NO_ERROR;
        }
        break;
        case GETPROPERTY:
        {
            CHECK_INTERFACE(ICellsFunctionToolService, data, reply);
            String16 name = data.readString16();
            String16 value;

            status_t result = getProperty(name, value);
            reply->writeString16(value);
            reply->writeInt32(result);
            return NO_ERROR;
        }
        break;
        case PIDNAMESPACE:
        {
            CHECK_INTERFACE(ICellsFunctionToolService, data, reply);
            int pid = data.readInt32();
            int hostpid,cellpid;
            int result = getPidNamespace(pid, &hostpid, &cellpid);
            reply->writeInt32(cellpid);
            reply->writeInt32(hostpid);
            reply->writeInt32(result);
            return NO_ERROR;
        }
        break;
        case SETENFORCE:
        {
            CHECK_INTERFACE(ICellsFunctionToolService, data, reply);
            int enforce = data.readInt32();

            status_t result = setEnforce(enforce);
            reply->writeInt32(result);
            return NO_ERROR;
        }
        break;
        case ISENFORCE:
        {
            CHECK_INTERFACE(ICellsFunctionToolService, data, reply);
            int enforce;
            int result = isEnforce(&enforce);
            reply->writeInt32(enforce);
            reply->writeInt32(result);
            return NO_ERROR;
        }
        break;
    }
    return BBinder::onTransact(code, data, reply, flags);
}

}
