###############################################################################
# DragonFire
LOCAL_PATH := $(call my-dir)

##########################
include $(CLEAR_VARS)
LOCAL_MODULE := iflyIME
LOCAL_MODULE_TAGS := optional
LOCAL_CERTIFICATE := PRESIGNED
LOCAL_DEX_PREOPT := false
LOCAL_MODULE_CLASS := APPS
LOCAL_MODULE_SUFFIX := $(COMMON_ANDROID_PACKAGE_SUFFIX)
LOCAL_SRC_FILES := iflyIME.apk
LOCAL_PREBUILT_JNI_LIBS := \
    lib/armeabi/libA3AEECD8.so  \
    lib/armeabi/libcom_iflytek_inputmethod_adsdk.so  \
    lib/armeabi/libcom_iflytek_inputmethod_aitalk.so  \
    lib/armeabi/libcom_iflytek_inputmethod_collect.so  \
    lib/armeabi/libcom_iflytek_inputmethod_intentengine.so  \
    lib/armeabi/libcom_iflytek_inputmethod_keyboardvoice.so  \
    lib/armeabi/libcom_iflytek_inputmethod_lck.so  \
    lib/armeabi/libcom_iflytek_inputmethod_lckac.so  \
    lib/armeabi/libcom_iflytek_inputmethod_minigame.so  \
    lib/armeabi/libcom_iflytek_inputmethod_mmp.so  \
    lib/armeabi/libcom_iflytek_inputmethod_mmrec.so  \
    lib/armeabi/libcom_iflytek_inputmethod_ocr.so  \
    lib/armeabi/libcom_iflytek_inputmethod_search.so  \
    lib/armeabi/libcom_iflytek_inputmethod_security.so  \
    lib/armeabi/libcom_iflytek_inputmethod_setting.so  \
    lib/armeabi/libcom_iflytek_inputmethod_settingsnew.so  \
    lib/armeabi/libcom_iflytek_inputmethod_thirdbkservices.so  \
    lib/armeabi/libcom_iflytek_inputmethod_wk.so  \
    lib/armeabi/libdexopt.so  \
    lib/armeabi/libEVad-v2.so  \
    lib/armeabi/libiflytek-opus.so  \
    lib/armeabi/liblwl.so  \
    lib/armeabi/libMetaVAD-v1.so  \
    lib/armeabi/libmsc-v9.so  \
    lib/armeabi/libNativeCrashCollect.so  \
    lib/armeabi/libsmartaiwrite-jni-v1048.so  \
    lib/armeabi/libspeex_fyj2.so  \
    lib/armeabi/libvipfont.so
LOCAL_MULTILIB :=32
#LOCAL_PROPRIETARY_MODULE := true
include $(BUILD_PREBUILT)

########################################
include $(CLEAR_VARS)
LOCAL_MODULE := chrome
LOCAL_MODULE_TAGS := optional
LOCAL_CERTIFICATE := platform
LOCAL_DEX_PREOPT := false
LOCAL_MODULE_CLASS := APPS
LOCAL_MODULE_SUFFIX := $(COMMON_ANDROID_PACKAGE_SUFFIX)
LOCAL_SRC_FILES := chrome.apk
#LOCAL_PROPRIETARY_MODULE := true
include $(BUILD_PREBUILT)

########################################
include $(CLEAR_VARS)
LOCAL_MODULE := hms
LOCAL_MODULE_TAGS := optional
LOCAL_CERTIFICATE := platform
LOCAL_DEX_PREOPT := false
LOCAL_MODULE_CLASS := APPS
LOCAL_MODULE_SUFFIX := $(COMMON_ANDROID_PACKAGE_SUFFIX)
LOCAL_SRC_FILES := hms.apk
LOCAL_PRIVATE_PLATFORM_APIS := true
LOCAL_PRIVILEGED_MODULE := true
LOCAL_USE_AAPT2 := true
#LOCAL_PROPRIETARY_MODULE := true
include $(BUILD_PREBUILT)
