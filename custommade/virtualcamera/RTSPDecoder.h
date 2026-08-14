#ifndef __RTSP_VIRTUALCAMERA_SERVICE_H__
#define __RTSP_VIRTUALCAMERA_SERVICE_H__

#include "DecoderBase.h"
#include <media/mediaplayer.h>
#include <media/IMediaPlayerService.h>

namespace android {

struct MyClient : public BnMediaPlayerClient {
    MyClient()
        : mEOS(false) {
    }

    virtual void notify(int msg, int ext1 __unused, int ext2 __unused, const Parcel *obj __unused) {
        Mutex::Autolock autoLock(mLock);

        if (msg == MEDIA_ERROR || msg == MEDIA_PLAYBACK_COMPLETE) {
            mEOS = true;
            mCondition.signal();
        }
    }

    void waitForEOS() {
        Mutex::Autolock autoLock(mLock);
        while (!mEOS) {
            mCondition.wait(mLock);
        }
    }

public:
    void stop() {
        Mutex::Autolock autoLock(mLock);
        mEOS = true;
        mCondition.signal();
    }

protected:
    virtual ~MyClient() {
    }

private:
    Mutex mLock;
    Condition mCondition;

    bool mEOS;

    DISALLOW_EVIL_CONSTRUCTORS(MyClient);
};

class RTSPDecoder :
    public DecoderBase {
  public:

    explicit RTSPDecoder(int id);
    virtual ~RTSPDecoder();

    virtual int CreateMediaSession();
    virtual int DestroyMediaSession();

  public:
    void SetPlayer(sp<IMediaPlayer> player) { mPlayer = player; }
    sp<IMediaPlayer> GetPlayer() const { return mPlayer; }
    void SetMyClient(sp<MyClient> client) { mClient = client; }
    sp<MyClient> GetMyClient() const { return mClient; }
    std::string GetUrl() const { return mUrl; }

  private:
    std::string mUrl;
    sp<IMediaPlayer> mPlayer;
    sp<MyClient> mClient;
};

}  // namespace android

#endif /*__RTSP_VIRTUALCAMERA_SERVICE_H__*/
