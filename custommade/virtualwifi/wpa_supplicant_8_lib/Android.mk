#
# Copyright (C) 2008 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
LOCAL_PATH := $(call my-dir)

# Use a custom libnl on releases before N
ifeq (0, $(shell test $(PLATFORM_SDK_VERSION) -lt 24; echo $$?))
EXTERNAL_GCE_LIBNL_INCLUDE := external/gce/libnl/include
else
EXTERNAL_GCE_LIBNL_INCLUDE :=
endif


WPA_SUPPL_DIR = $(LOCAL_PATH)/..
WPA_SRC_FILE :=

include $(WPA_SUPPL_DIR)/wpa_supplicant/android.config

WPA_SUPPL_DIR_INCLUDE = $(WPA_SUPPL_DIR)/wpa_supplicant/src \
	$(WPA_SUPPL_DIR)/wpa_supplicant/src/common \
	$(WPA_SUPPL_DIR)/wpa_supplicant/src/drivers \
	$(WPA_SUPPL_DIR)/wpa_supplicant/src/l2_packet \
	$(WPA_SUPPL_DIR)/wpa_supplicant/src/utils \
	$(WPA_SUPPL_DIR)/wpa_supplicant/src/wps \
	$(WPA_SUPPL_DIR)/wpa_supplicant \
	$(EXTERNAL_GCE_LIBNL_INCLUDE)

WPA_SUPPL_DIR_INCLUDE += external/libnl/include
WPA_SRC_FILE += driver_cmd_nl80211.c

ifeq ($(TARGET_ARCH),arm)
# To force sizeof(enum) = 4
L_CFLAGS += -mabi=aapcs-linux
endif

ifdef CONFIG_ANDROID_LOG
L_CFLAGS += -DCONFIG_ANDROID_LOG
endif

L_CFLAGS += -Wno-unused-parameter

########################

include $(CLEAR_VARS)
LOCAL_MODULE := lib_driver_cmd_cells
LOCAL_LICENSE_KINDS := SPDX-license-identifier-BSD
LOCAL_LICENSE_CONDITIONS := notice
LOCAL_VENDOR_MODULE := true
LOCAL_SHARED_LIBRARIES := libc libcutils liblog
LOCAL_HEADER_LIBRARIES := libcutils_headers

LOCAL_CFLAGS := $(L_CFLAGS) \
    $(GCE_VERSION_CFLAGS)

LOCAL_SRC_FILES := $(WPA_SRC_FILE)

LOCAL_C_INCLUDES := \
  $(WPA_SUPPL_DIR_INCLUDE)\

include $(BUILD_STATIC_LIBRARY)

########################
