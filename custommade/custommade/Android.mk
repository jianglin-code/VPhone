LOCAL_PATH := $(call my-dir)

#
# custommade service
#
include $(CLEAR_VARS)

LOCAL_CFLAGS :=

LOCAL_SRC_FILES:= \
	main_custommade.cpp 

LOCAL_MODULE := custommade
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_OWNER := cells
LOCAL_VENDOR_MODULE := true
LOCAL_SHARED_LIBRARIES := libm libcutils libc libutils liblog
#LOCAL_MODULE_PATH := $(TARGET_ROOT_OUT_SBIN)
include $(BUILD_EXECUTABLE)
