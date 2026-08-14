#ifndef __CELLSFUNCTIONTOOLSERVICE_H__
#define __CELLSFUNCTIONTOOLSERVICE_H__

#include "ICellsFunctionToolService.h"

namespace android
{

class String16;
class CellsFunctionToolService :
        public BinderService<CellsFunctionToolService>,
        public BnCellsFunctionToolService
{
    friend class BinderService<CellsFunctionToolService>;
public:

    CellsFunctionToolService();
    ~CellsFunctionToolService();

    static char const* getServiceName() { return "CellsFunctionToolService"; }

    virtual status_t setProperty(const String16& name,const String16& value);
    virtual status_t getProperty(const String16& name,String16& value);
    virtual int getPidNamespace(const int pid, int* hostpid, int* cellpid);
    virtual status_t setEnforce(const int enforce);
    virtual status_t isEnforce(int* enforce);

};
};
#endif
