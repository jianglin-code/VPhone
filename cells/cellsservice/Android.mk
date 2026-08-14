LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := libcellsbase
LOCAL_MODULE_TAGS := optional
LOCAL_SRC_FILES := CellsBase.cpp
LOCAL_SHARED_LIBRARIES := libm libcutils libc libutils liblog libbase
include $(BUILD_SHARED_LIBRARY)

#
# cells service
#
include $(CLEAR_VARS)

LOCAL_CFLAGS :=

LOCAL_SRC_FILES:= \
	CellsFunctionToolService.cpp \
	ICellsFunctionToolService.cpp \
	CellsPrivateService.cpp \
	ICellsPrivateService.cpp \
	CellsPrivateServiceutils.cpp \
	main_cells.cpp \
	../cells/cell_console.c

LOCAL_MODULE := cellsservice
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_OWNER := cells
LOCAL_SHARED_LIBRARIES := libm libcutils libc libbinder libutils libgui libpowermanager liblog libselinux
LOCAL_CPPFLAGS += -Wno-unused-variable -Wno-unused-parameter -Wno-unused-function
LOCAL_CFLAGS += -Wno-unused-variable -Wno-unused-parameter -Wno-unused-function
include $(BUILD_EXECUTABLE)

#sync
include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
	cellssync.cpp \
	ICellsPrivateService.cpp \
	ICellsFunctionToolService.cpp \

LOCAL_MODULE:= cellssync
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_OWNER := cells
#LOCAL_MODULE_PATH := $(TARGET_ROOT_OUT_SBIN)
LOCAL_SHARED_LIBRARIES := libm libcutils libc libbinder libutils liblog
include $(BUILD_EXECUTABLE)

#cellsswitch
include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
	cellsswitch.cpp \
	ICellsPrivateService.cpp

LOCAL_MODULE:= cellsswitch
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_OWNER := cells
#LOCAL_MODULE_PATH := $(TARGET_ROOT_OUT_SBIN)
LOCAL_SHARED_LIBRARIES := libm libcutils libc libbinder libutils liblog
include $(BUILD_EXECUTABLE)

#cellstrans
include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
	cellstrans.cpp

LOCAL_MODULE:= cellstrans
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_OWNER := cells
#LOCAL_MODULE_PATH := $(TARGET_ROOT_OUT_SBIN)
LOCAL_SHARED_LIBRARIES := libm libcutils libc libutils liblog
include $(BUILD_EXECUTABLE)

#start
include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
	cellsstart.cpp \
	ICellsPrivateService.cpp

LOCAL_MODULE:= cellsstart
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_OWNER := cells
#LOCAL_MODULE_PATH := $(TARGET_ROOT_OUT_SBIN)
LOCAL_SHARED_LIBRARIES := libm libcutils libc libbinder libutils liblog libbase
include $(BUILD_EXECUTABLE)

#sync
include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
	cellsready.cpp \
	ICellsPrivateService.cpp

LOCAL_MODULE:= cellsready
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_OWNER := cells
#LOCAL_MODULE_PATH := $(TARGET_ROOT_OUT_SBIN)
LOCAL_SHARED_LIBRARIES := libm libcutils libc libbinder libutils liblog
include $(BUILD_EXECUTABLE)

#console
include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
	cellsconsole.cpp \
	ICellsPrivateService.cpp \
	../cells/cell_console.c

LOCAL_MODULE:= cellsconsole
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_OWNER := cells
#LOCAL_MODULE_PATH := $(TARGET_ROOT_OUT_SBIN)
LOCAL_SHARED_LIBRARIES := libm libcutils libc libbinder libutils liblog
LOCAL_CFLAGS += -Wno-unused-variable -Wno-unused-parameter -Wno-unused-function
include $(BUILD_EXECUTABLE)

#init
include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
	cellsinit.cpp

LOCAL_MODULE:= cellsinit
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_OWNER := cells
#LOCAL_MODULE_PATH := $(TARGET_ROOT_OUT_SBIN)
LOCAL_SHARED_LIBRARIES := libm libcutils libc libutils liblog
include $(BUILD_EXECUTABLE)
