PRODUCT_PACKAGES += \
    android.hardware.sensors@2.1-service.multihal \
    android.hardware.sensors@2.1-impl \

#PRODUCT_COPY_FILES += \
#    vendor/custommade/virtualsensors/virtualsensors.rc:$(TARGET_COPY_OUT_VENDOR)/etc/init/virtualsensors.rc \
#    vendor/custommade/virtualsensors/virtualsensors.rc:$(TARGET_COPY_OUT_VENDOR)/etc/cells/vendor/virtualsensors.rc

# TODO(rkir):
# add a soong namespace and move this into a.h.sensors@2.1-impl
# as prebuilt_etc. For now soong_namespace causes a build break because the fw
# refers to our wifi HAL in random places.
PRODUCT_COPY_FILES += \
    vendor/custommade/virtualsensors/hals.conf:$(TARGET_COPY_OUT_VENDOR)/etc/sensors/hals.conf
