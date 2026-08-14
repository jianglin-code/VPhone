PRODUCT_PACKAGES += \
    android.hardware.gnss-service

PRODUCT_COPY_FILES += \
    vendor/custommade/virtualgps/virtualgps.rc:$(TARGET_COPY_OUT_VENDOR)/etc/init/virtualgps.rc \
    vendor/custommade/virtualgps/virtualgps.rc:$(TARGET_COPY_OUT_VENDOR)/etc/cells/vendor/virtualgps.rc

#PRODUCT_COPY_FILES += \
#    vendor/custommade/virtualgps/init.cells.gps.sh:$(TARGET_COPY_OUT_VENDOR)/bin/init.cells.gps.sh

PRODUCT_COPY_FILES += \
    frameworks/native/data/etc/android.hardware.location.gps.xml:$(TARGET_COPY_OUT_VENDOR)/etc/permissions/android.hardware.location.gps.xml

