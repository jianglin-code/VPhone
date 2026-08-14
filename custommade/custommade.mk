PRODUCT_PACKAGES += \
    custommade \
    custommadeapp \

# must !!!
PRODUCT_COPY_FILES += \
    vendor/custommade/custommadehost.rc:$(TARGET_COPY_OUT_VENDOR)/etc/init/custommadehost.rc \

PRODUCT_COPY_FILES += \
    vendor/custommade/custommade.rc:$(TARGET_COPY_OUT_VENDOR)/etc/init/custommade.rc \
    vendor/custommade/custommade.rc:$(TARGET_COPY_OUT_VENDOR)/etc/cells/vendor/custommade.rc \

PRODUCT_COPY_FILES += \
    vendor/custommade/init.custommade.init.sh:$(TARGET_COPY_OUT_VENDOR)/bin/init.custommade.init.sh \

PRODUCT_COPY_FILES += \
    $(call find-copy-subdir-files,*,vendor/custommade/custommade/simulations,$(TARGET_COPY_OUT_VENDOR)/etc/cells/custommade) \

# must be !!!
PRODUCT_PROPERTY_OVERRIDES += ro.boot.simulation=1

# need overwrite !!!
PRODUCT_PROPERTY_OVERRIDES += ro.hardware.real=rk30board
PRODUCT_PROPERTY_OVERRIDES += ro.boot.hardware.real=rk30board
PRODUCT_PROPERTY_OVERRIDES += ro.board.platform.real=rk3588
PRODUCT_PROPERTY_OVERRIDES += ro.target.product.real=tablet

PRODUCT_PROPERTY_OVERRIDES += ro.product.cpu.abi.real=arm64-v8a
PRODUCT_PROPERTY_OVERRIDES += ro.product.cpu.abilist.real=arm64-v8a,armeabi-v7a,armeabi
PRODUCT_PROPERTY_OVERRIDES += ro.vendor.product.cpu.abilist.real=arm64-v8a,armeabi-v7a,armeabi
PRODUCT_PROPERTY_OVERRIDES += ro.product.cpu.abilist32.real=armeabi-v7a,armeabi
PRODUCT_PROPERTY_OVERRIDES += ro.vendor.product.cpu.abilist32.real=armeabi-v7a,armeabi
PRODUCT_PROPERTY_OVERRIDES += ro.product.cpu.abilist64.real=arm64-v8a
PRODUCT_PROPERTY_OVERRIDES += ro.vendor.product.cpu.abilist64.real=arm64-v8a

PRODUCT_PROPERTY_OVERRIDES += ro.build.version.sdk.real=32
PRODUCT_PROPERTY_OVERRIDES += ro.build.version.release.real=12

include vendor/custommade/virtualwifi/virtualwifi.mk
include vendor/custommade/virtualcamera/virtualcamera.mk
include vendor/custommade/virtualbattery/virtualbattery.mk
include vendor/custommade/virtualgps/virtualgps.mk
include vendor/custommade/virtualsensors/virtualsensors.mk
include vendor/custommade/virtualface/virtualface.mk
include vendor/custommade/virtualfake/virtualfake.mk

#include vendor/custommade/zygisk/zygisk.mk
#PRODUCT_COPY_FILES += \
#    vendor/custommade/ksud.rc:$(TARGET_COPY_OUT_VENDOR)/etc/cells/vendor/ksud.rc \
