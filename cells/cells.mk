#add for ota update
DEVICE_VERSION := 2.0.0
ifeq ($(strip $(DEVICE_VERSION)),)
PRODUCT_PROPERTY_OVERRIDES += \
	ro.product.version=1.0
else
PRODUCT_PROPERTY_OVERRIDES += \
	ro.product.version=$(DEVICE_VERSION)
endif

######################
# fix apex compressed
######################
OVERRIDE_PRODUCT_COMPRESSED_APEX := false

PRODUCT_PACKAGES += \
	libcellsbase \
	celld \
	cellc \
	cellsservice \
	cellssync \
	cellstrans \
	cellsswitch \
	cellsstart \
	cellsconsole \
	cellsready \
	cellsinit \
	cellsapp

PRODUCT_PACKAGE_OVERLAYS += vendor/cells/overlay

PRODUCT_COPY_FILES += \
    vendor/cells/cellshost.rc:$(TARGET_COPY_OUT_VENDOR)/etc/init/cellshost.rc \
    vendor/cells/cellsvm.rc:$(TARGET_COPY_OUT_VENDOR)/etc/cells/vendor/cellsvm.rc \
    $(call find-copy-subdir-files,*,vendor/cells/cells-rc,$(TARGET_COPY_OUT_VENDOR)/etc/cells) \

PRODUCT_COPY_FILES += \
    vendor/cells/init.cells.overlayfs.sh:$(TARGET_COPY_OUT_SYSTEM)/bin/init.cells.overlayfs.sh \
    vendor/cells/init.cells.init.sh:$(TARGET_COPY_OUT_SYSTEM)/bin/init.cells.init.sh \

PRODUCT_COPY_FILES += \
    vendor/cells/adbhost.rc:$(TARGET_COPY_OUT_VENDOR)/etc/init/adbhost.rc \
    vendor/cells/adbhost1.rc:$(TARGET_COPY_OUT_VENDOR)/etc/init/adbhost1.rc \

include vendor/custommade/custommade.mk
include vendor/gapps/arm64/arm64-vendor.mk

# watchdog container do not need 
# /system_ext/etc/init/init.gs_watchdogd.rc
