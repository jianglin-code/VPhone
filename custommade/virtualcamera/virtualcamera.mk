PRODUCT_PACKAGES += \
    liblive555 \
    yuv4mpeg_to_v4l2 \
    yuv420_infiniteloop \
    v4l2loopback-ctl \
    virtualcamerainit \
    cellscamera \
    rtspstream \
    ffmpeg \

PRODUCT_COPY_FILES += \
    vendor/custommade/virtualcamera/virtualcamera.rc:$(TARGET_COPY_OUT_VENDOR)/etc/init/virtualcamera.rc \
    vendor/custommade/virtualcamera/virtualcamera.rc:$(TARGET_COPY_OUT_VENDOR)/etc/cells/vendor/virtualcamera.rc \
