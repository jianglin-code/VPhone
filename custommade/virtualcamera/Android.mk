LOCAL_PATH := $(call my-dir)

#
# live555 rtsp libs
#
include $(CLEAR_VARS)
LOCAL_MODULE := liblive555
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_OWNER := cells
LOCAL_VENDOR_MODULE := true
LIVE555_SOURCE_LIST := $(wildcard $(LOCAL_PATH)/JRTSPLIB/BasicUsageEnvironment/*.cpp)
LIVE555_SOURCE_LIST += $(wildcard $(LOCAL_PATH)/JRTSPLIB/groupsock/*.cpp)
LIVE555_SOURCE_LIST += $(wildcard $(LOCAL_PATH)/JRTSPLIB/groupsock/*.c)
LIVE555_SOURCE_LIST += $(wildcard $(LOCAL_PATH)/JRTSPLIB/liveMedia/*.cpp)
LIVE555_SOURCE_LIST += $(wildcard $(LOCAL_PATH)/JRTSPLIB/liveMedia/*.c)
LIVE555_SOURCE_LIST += $(wildcard $(LOCAL_PATH)/JRTSPLIB/UsageEnvironment/*.cpp)
LIVE555_SOURCE_LIST += $(wildcard $(LOCAL_PATH)/JRTSPLIB/android-ifaddrs/*.c)

LOCAL_SRC_FILES := $(LIVE555_SOURCE_LIST:$(LOCAL_PATH)/%=%)

LOCAL_C_INCLUDES:= $(LOCAL_PATH)/JRTSPLIB/BasicUsageEnvironment/include/ \
                   $(LOCAL_PATH)/JRTSPLIB/liveMedia/include/ \
                   $(LOCAL_PATH)/JRTSPLIB/groupsock/include/ \
                   $(LOCAL_PATH)/JRTSPLIB/UsageEnvironment/include/ \
                   $(LOCAL_PATH)/JRTSPLIB/android-ifaddrs/include/

LOCAL_SHARED_LIBRARIES := libcrypto libssl

LOCAL_CPPFLAGS := -fexceptions -DXLOCALE_NOT_USED=1 -DNULL=0 -DNO_SSTREAM=1 -UIP_ADD_SOURCE_MEMBERSHIP -DSOCKLEN_T=socklen_t -DNO_STD_LIB
LOCAL_CPPFLAGS += -Wno-unused-variable -Wno-unused-parameter
LOCAL_CFLAGS := -Wno-unused-variable -Wno-unused-parameter
include $(BUILD_SHARED_LIBRARY) 

#
# cellscamera service
#
include $(CLEAR_VARS)
RTP_SOURCE_LIST := $(wildcard $(LOCAL_PATH)/JRTPLIB/src/*.cpp)

LOCAL_SRC_FILES:= \
    main_virtualcamera.cpp  \
    VirtualCameraService.cpp  \
    IVirtualCameraService.cpp  \
    DecoderBase.cpp \
    RTPDecoder.cpp  \
    RTSPDecoder.cpp  \
    FileDecoder.cpp  \
    SimplePlayer.cpp \
    jthread/jmutex.cpp  \
    jthread/jthread.cpp  \
    Common/dtimenow.c  \
    Common/circular_list.c  \
    Common/network/NetworkSocket.c  \
    Common/thread/linux/mutex_pthread.c  \
    Common/thread/linux/thread_pthread.c  \
    Common/thread/linux/Semaphore_linux.c  

LOCAL_SRC_FILES += $(RTP_SOURCE_LIST:$(LOCAL_PATH)/%=%)

LOCAL_MODULE := cellscamera
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_OWNER := cells
#LOCAL_VENDOR_MODULE := true
LOCAL_SHARED_LIBRARIES := libm libcutils libc libbinder libutils liblog libnativewindow libui libandroid \
         libstagefright libstagefright_foundation libmedia libmedia_omx libaudioclient libbase libgui

LOCAL_HEADER_LIBRARIES := \
        libmediadrm_headers \
        libmediametrics_headers \

LOCAL_C_INCLUDES:= \
        frameworks/av/media/libstagefright \
        frameworks/native/include/media/openmax

LOCAL_CPPFLAGS := -Wno-unused-variable -Wno-unused-parameter -Wno-unused-function
LOCAL_CFLAGS := -Wno-unused-variable -Wno-unused-parameter -Wno-unused-function
include $(BUILD_EXECUTABLE)

#v4l2loopback-ctl
include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
	v4l2loopback-ctl.c 

LOCAL_MODULE:= v4l2loopback-ctl
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_OWNER := cells
LOCAL_VENDOR_MODULE := true
#LOCAL_MODULE_PATH := $(TARGET_ROOT_OUT_SBIN)
#LOCAL_SHARED_LIBRARIES := libm libcutils libc libbinder libutils liblog
include $(BUILD_EXECUTABLE)

#yuv420_infiniteloop
include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
	yuv420_infiniteloop.c 

LOCAL_MODULE:= yuv420_infiniteloop
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_OWNER := cells
LOCAL_VENDOR_MODULE := true
#LOCAL_MODULE_PATH := $(TARGET_ROOT_OUT_SBIN)
#LOCAL_SHARED_LIBRARIES := libm libcutils libc libbinder libutils liblog
include $(BUILD_EXECUTABLE)

#yuv4mpeg_to_v4l2
include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
	yuv4mpeg_to_v4l2.c 

LOCAL_MODULE:= yuv4mpeg_to_v4l2
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_OWNER := cells
LOCAL_VENDOR_MODULE := true
#LOCAL_MODULE_PATH := $(TARGET_ROOT_OUT_SBIN)
#LOCAL_SHARED_LIBRARIES := libm libcutils libc libbinder libutils liblog
include $(BUILD_EXECUTABLE)

#
# rtspstream
#
include $(CLEAR_VARS)

LOCAL_SRC_FILES:= \
        RTSPStream.cpp  \
        CallbackProcessor.cpp \
        SimplePlayer.cpp \

LOCAL_MODULE := rtspstream
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_OWNER := cells
#LOCAL_VENDOR_MODULE := true

LOCAL_HEADER_LIBRARIES := \
        libmediadrm_headers \
        libmediametrics_headers \

LOCAL_C_INCLUDES:= \
        frameworks/av/media/libstagefright \
        frameworks/native/include/media/openmax

LOCAL_SHARED_LIBRARIES := libm libcutils libc libbinder libutils liblog libnativewindow libui libandroid \
        libstagefright libstagefright_foundation libmedia libmedia_omx libaudioclient libbase libgui

include $(BUILD_EXECUTABLE)

#
# virtualcamerainit tools
#
include $(CLEAR_VARS)

LOCAL_CFLAGS :=

LOCAL_SRC_FILES:= \
	virtualcamerainit.cpp 

LOCAL_MODULE := virtualcamerainit
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_OWNER := cells
LOCAL_VENDOR_MODULE := true
LOCAL_SHARED_LIBRARIES := libm libcutils libc libutils liblog
#LOCAL_MODULE_PATH := $(TARGET_ROOT_OUT_SBIN)
include $(BUILD_EXECUTABLE)

# ffmepg dependency: /system/libandroid
include $(CLEAR_VARS)
LOCAL_MODULE := ffmpeg
LOCAL_MODULE_CLASS := EXECUTABLES
#LOCAL_SHARED_LIBRARIES := libc libdl liblog
LOCAL_MODULE_STEM := $(LOCAL_MODULE)
LOCAL_SRC_FILES := ffmepg/$(LOCAL_MODULE)
LOCAL_CHECK_ELF_FILES := false
include $(BUILD_PREBUILT)
