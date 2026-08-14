PRODUCT_PACKAGES += \
    android.hardware.power-service.example \
    android.hardware.power.stats-service.example

PRODUCT_COPY_FILES += \
    vendor/custommade/virtualbattery/virtualbattery.rc:$(TARGET_COPY_OUT_VENDOR)/etc/init/virtualbattery.rc \
    vendor/custommade/virtualbattery/virtualbattery.rc:$(TARGET_COPY_OUT_VENDOR)/etc/cells/vendor/virtualbattery.rc

PRODUCT_COPY_FILES += \
    vendor/custommade/virtualbattery/init.cells.battery.sh:$(TARGET_COPY_OUT_VENDOR)/bin/init.cells.battery.sh \
    $(call find-copy-subdir-files,*,vendor/custommade/virtualbattery/power_supply,$(TARGET_COPY_OUT_VENDOR)/etc/cells/custommade/hw/battery/power_supply)
