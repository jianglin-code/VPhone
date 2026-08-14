#define LOG_TAG "VIRTUALCAMERA"
#include <cutils/log.h>

#include <android-base/properties.h>

#include "VirtualCameraService.h"

#include "DecoderBase.h"
#include "RTPDecoder.h"
#include "RTSPDecoder.h"
#include "FileDecoder.h"

namespace android {

VirtualCameraService::VirtualCameraService()
{

}

VirtualCameraService::~VirtualCameraService()
{

}

status_t VirtualCameraService::createSession(int id)
{
    Mutex::Autolock l(mInputMutex);
    ALOGD("%s", __FUNCTION__);

    DecoderBase* decoder = GetDecoderBase(id);
    if(decoder){
        decoder->CreateMediaSession();
    }
    return NO_ERROR;
}

status_t VirtualCameraService::destroySession(int id)
{
    Mutex::Autolock l(mInputMutex);
    ALOGD("%s", __FUNCTION__);

    DecoderBase* decoder = GetDecoderBase(id);
    if(decoder){
        decoder->DestroyMediaSession();
    }
    return NO_ERROR;
}

status_t VirtualCameraService::setSurface(int id, const sp<IGraphicBufferProducer>& bufferProducer, int32_t width, int32_t height, int32_t format, int32_t transform)
{
    Mutex::Autolock l(mInputMutex);
    ALOGD("%s", __FUNCTION__);

    sp<Surface> window;

    if (bufferProducer != 0) {
        window = new Surface(bufferProducer, true);
        ALOGD("setSurface width = %d , height = %d , format = %d , transform = %d ", width, height, format, transform);
    }

    DecoderBase* decoder = GetAndCreateDecoderBase(id);
    if (decoder) {
        decoder->SetSurface(window, width, height, format, transform);
    }
    return NO_ERROR;
}

status_t VirtualCameraService::releaseSurface(int id)
{
    Mutex::Autolock l(mInputMutex);
    ALOGD("%s", __FUNCTION__);

    DecoderBase* decoder = GetDecoderBase(id);
    if(decoder){
        decoder->SetSurface(nullptr, -1, -1, -1, 0);
        decoder->SetCallbackSurface(nullptr, -1, -1, -1, 0);
        RemoveDecoderBase(id);
    }
    return NO_ERROR;
}

status_t VirtualCameraService::setCallBackSurface(int id, const sp<IGraphicBufferProducer>& bufferProducer, int32_t width, int32_t height, int32_t format, int32_t transform)
{
    Mutex::Autolock l(mInputMutex);
    ALOGD("%s", __FUNCTION__);

    sp<Surface> window;
    status_t res;

    int32_t newformat = HAL_PIXEL_FORMAT_YCbCr_420_888;

    if (bufferProducer != 0) {
        window = new Surface(bufferProducer, true);

        ANativeWindow_Buffer buffer;
        if (ANativeWindow_lock(window.get(), &buffer, NULL) == 0) {
            ALOGD("CallBack width = %d , height = %d , stride = %d ", buffer.width, buffer.height, buffer.stride);
            ANativeWindow_setBuffersGeometry(window.get(), width, height, newformat); 
            ANativeWindow_unlockAndPost(window.get());
        }
    }

    DecoderBase* decoder = GetDecoderBase(id);
    if (decoder) {
        decoder->SetCallbackSurface(window, width, height, newformat, transform);
    }
    return NO_ERROR;
}

status_t VirtualCameraService::releaseCallBackSurface(int id)
{
    Mutex::Autolock l(mInputMutex);
    ALOGD("%s", __FUNCTION__);

    DecoderBase* decoder = GetDecoderBase(id);
    if(decoder){
        decoder->SetCallbackSurface(nullptr, -1, -1, -1, 0);
    }
    return NO_ERROR;
}

DecoderBase* VirtualCameraService::GetDecoderBase(int id)
{
    for(Vector<DecoderBase*>::iterator 
        it = mDecoderList.begin(); it != mDecoderList.end(); it++){
        if(id == (*it)->GetDecoderID()){
            return *it;
        }
    }
    return NULL;
}

int VirtualCameraService::RemoveDecoderBase(int id)
{
    for(int index = 0; index < mDecoderList.size(); index++){
        if(id == mDecoderList[index]->GetDecoderID()){
            DecoderBase* decoder = mDecoderList[index];
            int ret = mDecoderList.removeAt(index);
            ALOGD("Remove GetDecoderID = %d", decoder->GetDecoderID());
            delete decoder;
            return ret;
        }
    }

    return -1;
}

int VirtualCameraService::AddDecoderBase(DecoderBase* decoder)
{
    if(decoder == NULL)
        return -1;

    Vector<DecoderBase*>::iterator it;
    for(it= mDecoderList.begin(); it != mDecoderList.end(); it++) {
        if(decoder->GetDecoderID() == (*it)->GetDecoderID()){
            break;
        }
    }

    if(it == mDecoderList.end()){
        ALOGD("Add GetDecoderID = %d", decoder->GetDecoderID());
        return mDecoderList.add(decoder);
    }else{
        return -1;
    }
}

DecoderBase* VirtualCameraService::GetAndCreateDecoderBase(int id)
{
    for(Vector<DecoderBase*>::iterator 
        it = mDecoderList.begin(); it != mDecoderList.end(); it++){
        if(id == (*it)->GetDecoderID()){
            return *it;
        }
    }

    std::string ssession = android::base::GetProperty("persist.sys.virtualcamera.session", "rtp");
    if (ssession == "rtp") {
        RTPDecoder* rtp = new RTPDecoder(id);
        if (AddDecoderBase((DecoderBase *)rtp) >= 0){
            return (DecoderBase *)rtp;
        }else{
            delete rtp;
        }
    } else if (ssession == "file") {
        FileDecoder* file = new FileDecoder(id);
        if (AddDecoderBase((DecoderBase *)file) >= 0){
            return (DecoderBase *)file;
        }else{
            delete file;
        }
    } else if (ssession == "rtsp") {
        RTSPDecoder* rtsp = new RTSPDecoder(id);
        if (AddDecoderBase((DecoderBase *)rtsp) >= 0){
            return (DecoderBase *)rtsp;
        }else{
            delete rtsp;
        }
    }
    return NULL;
}

};
