# celld Makefile
#
# Copyright (C) 2011-2013 Columbia University
# Author: Jeremy C. Andrus <jeremya@cs.columbia.edu>
#
LOCAL_PATH := $(call my-dir)

#
# celld (container control daemon)
#
include $(CLEAR_VARS)

LOCAL_SRC_FILES:= \
	cell_console.c \
	nsexec.c \
	shared_ops.c \
	util.c \
	celld.c \
	cell_config.c \
	cellnetwork.c \
	array.c \

LOCAL_MODULE := celld
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_OWNER := cells
LOCAL_SHARED_LIBRARIES := libm libcutils libc libselinux liblog libcap
LOCAL_CFLAGS += -Wno-unused-variable -Wno-unused-parameter -Wno-unused-function
#LOCAL_MODULE_PATH := $(TARGET_ROOT_OUT_SBIN)
include $(BUILD_EXECUTABLE)

#
# cell (container control front-end)
#
include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
	cell_console.c \
	shared_ops.c \
	util.c \
	cell.c

LOCAL_MODULE:= cellc
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_OWNER := cells
LOCAL_SHARED_LIBRARIES := libm libcutils libc liblog
LOCAL_CFLAGS += -Wno-unused-variable -Wno-unused-parameter -Wno-unused-function
include $(BUILD_EXECUTABLE)
