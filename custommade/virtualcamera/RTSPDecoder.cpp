#define LOG_TAG "VIRTUALCAMERA-RTSP"
#include <cutils/log.h>

#include <android-base/properties.h>
#include <binder/IServiceManager.h>
#include <binder/ProcessState.h>
#include <media/IMediaHTTPService.h>

#include "RTSPDecoder.h"

using namespace android;

static void thread_rtsp_virtualcamera(void * data) 
{
    RTSPDecoder* decoder = (RTSPDecoder*)data;
    if (decoder == NULL)
        return;

    sp<IServiceManager> sm = defaultServiceManager();
    sp<IBinder> binder = sm->getService(String16("media.player"));
    if(binder == nullptr)
        return;
    sp<IMediaPlayerService> service = interface_cast<IMediaPlayerService>(binder);
    if(service == nullptr)
        return;

    ProcessState::self()->startThreadPool();

    sp<MyClient> mClient = new MyClient;
    decoder->SetMyClient(mClient);

    sp<IMediaPlayer> mPlayer = service->create(mClient, AUDIO_SESSION_ALLOCATE);
    decoder->SetPlayer(mPlayer);

    ALOGD("RTSP URL = %s", decoder->GetUrl().c_str());

    if (mPlayer != NULL && 
        mPlayer->setDataSource(NULL, decoder->GetUrl().c_str(), NULL) == NO_ERROR) {

        mPlayer->setVideoSurfaceTexture(decoder->GetSurface()->getIGraphicBufferProducer());
        mPlayer->prepareAsync();
        mPlayer->start();

        ALOGD("CreateMediaSession RTSP Player BEGIN");
        mClient->waitForEOS();
        ALOGD("CreateMediaSession RTSP Player END");
    }
}

RTSPDecoder::RTSPDecoder(int id)
    :DecoderBase(id) {

}

RTSPDecoder::~RTSPDecoder() {

}

int RTSPDecoder::CreateMediaSession()
{
    if(mSurface == nullptr){
        ALOGE("Surface is null");
        return 0;
    }

    if(mClient != NULL)
        return 0;

    mUrl = android::base::GetProperty("persist.sys.virtualcamera.session.rtspurl", "rtsp://192.168.1.6/live");
	int rtspfd = open("/data/.virtualcameraip",O_RDONLY);
	if (rtspfd >= 0) {
		char url[256] = {0};
		int len = read(rtspfd, url, 256);
		if (len > 0 && strncasecmp("rtsp://", url, 7) == 0) {
            mUrl = std::string(url, len-1);
            ALOGD("rtspurl = %s", mUrl.c_str());
		}
		close(rtspfd);
	}

    msRecvQuit = 0;

    ALOGD("CreateMediaSession BEGIN");
    msRecvThread = Thread_Create(thread_rtsp_virtualcamera, this);
    Thread_Run(msRecvThread);
    ALOGD("CreateMediaSession END");
    return 0;
}

int RTSPDecoder::DestroyMediaSession()
{
    if(mClient == NULL)
        return 0;

    msRecvQuit = 1;

    ALOGD("DestroyMediaSession BEGIN");
    mPlayer->stop();
    mPlayer = nullptr;
    mClient->stop();
    mClient = nullptr;

    Thread_Destroy(msRecvThread);
    msRecvThread = NULL;
    ALOGD("DestroyMediaSession END");
    return 0;
}
