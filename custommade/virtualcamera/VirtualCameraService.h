#ifndef __VIRTUALCAMERA_SERVICE_H__
#define __VIRTUALCAMERA_SERVICE_H__

#include <gui/Surface.h>
#include <utils/Vector.h>

#include "IVirtualCameraService.h"
#include "DecoderBase.h"

namespace android
{

class String16;
class VirtualCameraService :
        public BinderService<VirtualCameraService>,
        public BnVirtualCameraService
{
    friend class BinderService<VirtualCameraService>;
public:
    VirtualCameraService();
    ~VirtualCameraService();

    static char const* getServiceName() { return "cells.camera"; }

    virtual status_t createSession(int id);
    virtual status_t destroySession(int id);
    virtual status_t setSurface(int id, const sp<IGraphicBufferProducer>& bufferProducer, int32_t width, int32_t height, int32_t format, int32_t transform);
    virtual status_t releaseSurface(int id);
    virtual status_t setCallBackSurface(int id, const sp<IGraphicBufferProducer>& bufferProducer, int32_t width, int32_t height, int32_t format, int32_t transform);
    virtual status_t releaseCallBackSurface(int id);

private:
    DecoderBase* GetDecoderBase(int id);
    DecoderBase* GetAndCreateDecoderBase(int id);
    int RemoveDecoderBase(int id);
    int AddDecoderBase(DecoderBase* decoder);

private:
    Vector<DecoderBase*> mDecoderList;
    Mutex mInputMutex;
};
};
#endif
