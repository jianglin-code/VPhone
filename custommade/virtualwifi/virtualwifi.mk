######################
# wifi config
######################
PRODUCT_PACKAGES += \
    iw \
    create_radios2 \
    createns2 \
    dhcpclient2 \
    dhcpserver2 \
    execns2 \
    ipv6proxy2 \
    hostapd_cells \
    hostapd_cells_nohidl \
    libhostapd_cells_hidl \
    wpa_cells_supplicant \
    libwpa_cells_hidl \
    lib_driver_cmd_cells \
    libwifi-hal-cells \

PRODUCT_COPY_FILES += \
    vendor/custommade/virtualwifi/virtualwifi.rc:$(TARGET_COPY_OUT_VENDOR)/etc/cells/vendor/virtualwifi.rc \

PRODUCT_COPY_FILES += \
    vendor/custommade/virtualwifi/init.cells.wifi.sh:$(TARGET_COPY_OUT_VENDOR)/bin/init.cells.wifi.sh \
    vendor/custommade/virtualwifi/hostapd.conf:$(TARGET_COPY_OUT_VENDOR)/etc/cells/custommade/wifi/hostapd.conf \
    vendor/custommade/virtualwifi/wpa_supplicant.conf:$(TARGET_COPY_OUT_VENDOR)/etc/cells/custommade/wifi/wpa_supplicant.conf \

WIFISIM_KO_FILE := $(TOPDIR)$(PRODUCT_KERNEL_PATH)/drivers/net/wireless/mac80211_hwsim.ko
WIFIVIRT_KO_FILE := $(TOPDIR)$(PRODUCT_KERNEL_PATH)/drivers/net/wireless/virt_wifi.ko

PRODUCT_COPY_FILES += \
    $(WIFISIM_KO_FILE):$(TARGET_COPY_OUT_VENDOR)/etc/cells/mac80211_hwsim.ko \
    $(WIFIVIRT_KO_FILE):$(TARGET_COPY_OUT_VENDOR)/etc/cells/virt_wifi.ko \
