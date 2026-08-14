LOCAL_PATH := $(call my-dir)

#
# cells android.hardware.camera.flash@2.1-service
#
include $(CLEAR_VARS)

LOCAL_CFLAGS :=

LOCAL_SRC_FILES:= \
	fake1.cpp \
	IOPPOCoreService.cpp \
	OPPOCoreService.cpp

LOCAL_MODULE := android.hardware.camera.flash@2.1-service
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_OWNER := cells
#LOCAL_VENDOR_MODULE := true
#LOCAL_MODULE_PATH := $(TARGET_OUT_VENDOR)/bin/hw
LOCAL_SHARED_LIBRARIES := libm libcutils libc libbinder libutils liblog libbase
LOCAL_CPPFLAGS += -Wno-unused-variable -Wno-unused-parameter -Wno-unused-function
LOCAL_CFLAGS += -Wno-unused-variable -Wno-unused-parameter -Wno-unused-function
include $(BUILD_EXECUTABLE)

#
# cells android.hardware.telephony@4.2-service
#
include $(CLEAR_VARS)

LOCAL_CFLAGS :=

LOCAL_SRC_FILES:= \
	fake2.cpp \
	IOPPOToolsService.cpp \
	OPPOToolsService.cpp

LOCAL_MODULE := android.hardware.telephony@4.2-service
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_OWNER := cells
#LOCAL_VENDOR_MODULE := true
#LOCAL_MODULE_PATH := $(TARGET_OUT_VENDOR)/bin/hw
LOCAL_SHARED_LIBRARIES := libm libcutils libc libbinder libutils liblog libbase
LOCAL_CPPFLAGS += -Wno-unused-variable -Wno-unused-parameter -Wno-unused-function
LOCAL_CFLAGS += -Wno-unused-variable -Wno-unused-parameter -Wno-unused-function
include $(BUILD_EXECUTABLE)
