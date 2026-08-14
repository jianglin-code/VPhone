LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE := y
LOCAL_MODULE_CLASS := APPS
LOCAL_MODULE_TAGS := optional
LOCAL_BUILT_MODULE_STEM := package.apk
LOCAL_MODULE_SUFFIX := $(COMMON_ANDROID_PACKAGE_SUFFIX)
#LOCAL_PRIVILEGED_MODULE :=
LOCAL_CERTIFICATE := PRESIGNED
LOCAL_DEX_PREOPT := false
#LOCAL_OVERRIDES_PACKAGES :=
LOCAL_SRC_FILES := $(LOCAL_MODULE).apk
LOCAL_ENFORCE_USES_LIBRARIES := false
ifeq ($(strip $(TARGET_ARCH)), arm)
LOCAL_PREBUILT_JNI_LIBS := \
    lib/armeabi/libblurdetector.so    \
    lib/armeabi/libbs-sec.so    \
    lib/armeabi/libbsdiff.so    \
    lib/armeabi/libbuglybacktrace.so    \
    lib/armeabi/libbugly_dumper.so    \
    lib/armeabi/libBugly_Native.so    \
    lib/armeabi/libdaemon_acc_v2.2.4.so    \
    lib/armeabi/libdce-1.1.18-mfr.so    \
    lib/armeabi/libdeflater7z.so    \
    lib/armeabi/libfg.so    \
    lib/armeabi/libhpatch.so    \
    lib/armeabi/libjpegenc.so    \
    lib/armeabi/libmarsxlog.so    \
    lib/armeabi/libmmkv.so    \
    lib/armeabi/libps.so    \
    lib/armeabi/libqimei.so    \
    lib/armeabi/libQQImageCompare-1.6-mfr.so    \
    lib/armeabi/librdefense.so    \
    lib/armeabi/librmonitor_base.so    \
    lib/armeabi/librmonitor_memory.so    \
    lib/armeabi/libsm_mq.so    \
    lib/armeabi/libthread_sched.so    \
    lib/armeabi/libtmf-gm.so    \
    lib/armeabi/libTmsdk-2.0.13-mfr.so    \
    lib/armeabi/libturingbase.so    \
    lib/armeabi/libturingvd.so    \
    lib/armeabi/libvbhook.so    \
    lib/armeabi/libyyb_csech.so    \
    lib/armeabi/libz.so
else ifeq ($(strip $(TARGET_ARCH)), arm64)
LOCAL_PREBUILT_JNI_LIBS := \
    lib/arm64-v8a/libblurdetector.so    \
    lib/arm64-v8a/libbs-sec.so    \
    lib/arm64-v8a/libbsdiff.so    \
    lib/arm64-v8a/libbuglybacktrace.so    \
    lib/arm64-v8a/libbugly_dumper.so    \
    lib/arm64-v8a/libBugly_Native.so    \
    lib/arm64-v8a/libdaemon_acc_v2.2.4.so    \
    lib/arm64-v8a/libdce-1.1.18-mfr.so    \
    lib/arm64-v8a/libdeflater7z.so    \
    lib/arm64-v8a/libfg.so    \
    lib/arm64-v8a/libhpatch.so    \
    lib/arm64-v8a/libjpegenc.so    \
    lib/arm64-v8a/libmarsxlog.so    \
    lib/arm64-v8a/libmmkv.so    \
    lib/arm64-v8a/libpcdn.so    \
    lib/arm64-v8a/libpcdn_wrap.so    \
    lib/arm64-v8a/libps.so    \
    lib/arm64-v8a/libqimei.so    \
    lib/arm64-v8a/libQQImageCompare-1.6-mfr.so    \
    lib/arm64-v8a/librdefense.so    \
    lib/arm64-v8a/librmonitor_base.so    \
    lib/arm64-v8a/librmonitor_memory.so    \
    lib/arm64-v8a/libsm_mq.so    \
    lib/arm64-v8a/libthread_sched.so    \
    lib/arm64-v8a/libtmf-gm.so    \
    lib/arm64-v8a/libTmsdk-2.0.13-mfr.so    \
    lib/arm64-v8a/libturingbase.so    \
    lib/arm64-v8a/libturingvd.so    \
    lib/arm64-v8a/libvbhook.so    \
    lib/arm64-v8a/libyyb_csech.so    \
    lib/arm64-v8a/libz.so
endif
#LOCAL_REQUIRED_MODULES :=
#LOCAL_PREBUILT_JNI_LIBS :=
include $(BUILD_PREBUILT)
