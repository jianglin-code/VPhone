#ifndef __FILE_VIRTUALCAMERA_SERVICE_H__
#define __FILE_VIRTUALCAMERA_SERVICE_H__

#include "DecoderBase.h"
#include <media/stagefright/foundation/ALooper.h>
#include "SimplePlayer.h"

namespace android {

class FileDecoder :
    public DecoderBase {
  public:

    explicit FileDecoder(int id);
    virtual ~FileDecoder();

    virtual int CreateMediaSession();
    virtual int DestroyMediaSession();

  public:
    void SetPlayer(sp<SimplePlayer> player) { mFilePlayer = player; }
    sp<SimplePlayer> GetPlayer() const { return mFilePlayer; }
    void SetALooper(sp<ALooper> looper) { mLooper = looper; }
    sp<ALooper> GetALooper() const { return mLooper; }
    std::string GetFilePath() const { return mFilePath; }

  private:
    sp<SimplePlayer> mFilePlayer;
    sp<ALooper> mLooper;
    std::string mFilePath;
};

}  // namespace android

#endif /* __FILE_VIRTUALCAMERA_SERVICE_H__ */