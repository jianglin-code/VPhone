#ifndef __ICELLSFUNCTIONTOOLSERVICE_H__
#define __ICELLSFUNCTIONTOOLSERVICE_H__

#include <binder/IInterface.h>
#include <binder/Parcel.h>
#include <binder/BinderService.h>
#include <cutils/properties.h>
#include <utils/String16.h>

namespace android
{

class String16;
class ICellsFunctionToolService : public IInterface
{
protected:
    enum {
        SETPROPERTY = IBinder::FIRST_CALL_TRANSACTION,
        GETPROPERTY = IBinder::FIRST_CALL_TRANSACTION + 1,
        PIDNAMESPACE = IBinder::FIRST_CALL_TRANSACTION + 2,
        SETENFORCE = IBinder::FIRST_CALL_TRANSACTION + 3,
        ISENFORCE = IBinder::FIRST_CALL_TRANSACTION + 4,
    };

public:
    DECLARE_META_INTERFACE(CellsFunctionToolService)

    virtual status_t setProperty(const String16& name,const String16& value) = 0;
    virtual status_t getProperty(const String16& name,String16& value) = 0;
    virtual int getPidNamespace(const int pid, int* hostpid, int* cellpid) = 0;
    virtual status_t setEnforce(const int enforce) = 0;
    virtual status_t isEnforce(int* enforce) = 0;
};

class BnCellsFunctionToolService : public BnInterface<ICellsFunctionToolService>
{
    virtual status_t onTransact(uint32_t code,
                                const Parcel& data,
                                Parcel* reply,
                                uint32_t flags = 0);
};

}

#endif
