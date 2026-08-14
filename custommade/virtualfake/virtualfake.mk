PRODUCT_PACKAGES += \
    android.hardware.camera.flash@2.1-service \
    android.hardware.telephony@4.2-service \

PRODUCT_COPY_FILES += \
    vendor/custommade/virtualfake/virtualfake.rc:$(TARGET_COPY_OUT_VENDOR)/etc/init/virtualfake.rc \
    vendor/custommade/virtualfake/virtualfake.rc:$(TARGET_COPY_OUT_VENDOR)/etc/cells/vendor/virtualfake.rc \
