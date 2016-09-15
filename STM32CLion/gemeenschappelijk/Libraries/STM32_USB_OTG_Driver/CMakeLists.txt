set(USBOTGDriverBestanden src/usb_core.c)
if (USE_USBOTG_DEVICE EQUAL 1)
    set(USBOTGDriverBestanden ${USBOTGDriverBestanden} src/usb_dcd.c src/usb_dcd_int.c)
endif ()

if (USE_USBOTG_HOST EQUAL 1)
    set(USBOTGDriverBestanden ${USBOTGDriverBestanden} src/usb_hcd.c src/usb_hcd_int.c)
endif ()

include_directories(./inc)

add_library(STM32USBOTGDriver ${USBOTGDriverBestanden})


