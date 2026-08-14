#define LOG_TAG "VIRTUALCAMERA-File"
#include <cutils/log.h>

#include <android-base/properties.h>
#include <binder/ProcessState.h>

#include "FileDecoder.h"

using namespace android;

static void thread_files_virtualcamera(void * data) 
{
    FileDecoder* decoder = (FileDecoder*)data;
    if(decoder == NULL)
        return;

    ProcessState::self()->startThreadPool();

    ALOGD("files path = %s", decoder->GetFilePath().c_str());

    sp<ALooper> looper = new android::ALooper;
    decoder->SetALooper(looper);
    looper->start();

    sp<SimplePlayer> mSimplePlayer = new SimplePlayer;
    decoder->SetPlayer(mSimplePlayer);
    looper->registerHandler(mSimplePlayer);

    mSimplePlayer->setDataSource(decoder->GetFilePath().c_str());
    mSimplePlayer->setSurface(decoder->GetSurface()->getIGraphicBufferProducer());
    mSimplePlayer->start();

    ALOGD("CreateMediaSession File Player BEGIN");
    while(!decoder->GetRecvQuit()) sleep(1);
    ALOGD("CreateMediaSession File Player END");
}

FileDecoder::FileDecoder(int id)
    :DecoderBase(id) {

}

FileDecoder::~FileDecoder() {

}

int FileDecoder::CreateMediaSession()
{
    if(mSurface == nullptr){
        ALOGE("Surface is null");
        return 0;
    }

    if(mLooper != NULL)
        return 0;

    mFilePath = android::base::GetProperty("persist.sys.virtualcamera.session.filepath", "/data/video.mp4");
	int fd = open("/data/.virtualcameraip",O_RDONLY);
	if (fd >= 0) {
		char path[256] = {0};
		int len = read(fd, path, 256);
		if (len > 0 && strncasecmp(".mp4", (((char*)path)+(len-1-4)), 4) == 0) {
            mFilePath = std::string(path, len-1);
            ALOGD("mFilePath = %s", mFilePath.c_str());
		}
		close(fd);
	}

    msRecvQuit = 0;

    ALOGD("FileCreateMediaSession BEGIN");
    msRecvThread = Thread_Create(thread_files_virtualcamera, this);
    Thread_Run(msRecvThread);
    ALOGD("FileCreateMediaSession END");
    return 0;
}

int FileDecoder::DestroyMediaSession() {
    if(mLooper == NULL)
        return 0;

    msRecvQuit = 1;

    ALOGD("FileDestroyMediaSession BEGIN");
    mFilePlayer->stop();
    mFilePlayer->reset();
    mFilePlayer = nullptr;

    mLooper->stop();
    mLooper = nullptr;

    Thread_Destroy(msRecvThread);
    msRecvThread = NULL;
    ALOGD("FileDestroyMediaSession END");
    return 0;
}
