
FILE (GLOB USBDeviceCoreBestanden Core/src/*.c)
include_directories(./Core/inc)

if (USE_USBDEVICE_CDC EQUAL 1)
FILE (GLOB USBDeviceBestanden Class/cdc/src/*.c)
include_directories(./Class/cdc/inc)
endif()

if (USE_USBDEVICE_HID EQUAL 1)
FILE (GLOB USBDeviceBestanden Class/hid/src/*.c)
include_directories(./Class/hid/inc)
endif()

if (USE_USBDEVICE_MSC EQUAL 1)
FILE (GLOB USBDeviceBestanden Class/msc/src/*.c)
include_directories(./Class/msc/inc)

endif()

if (USE_USBDEVICE_DFU EQUAL 1)
FILE (GLOB USBDeviceBestanden Class/dfu/src/*.c )
include_directories(./Class/dfu/inc)
endif()

if (USE_USBDEVICE_AUDIO EQUAL 1)
FILE (GLOB USBDeviceBestanden Class/audio/src/*.c)
include_directories(./Class/audio/inc )
endif()


add_library(STM32USBDeviceLibrary ${USBDeviceBestanden} ${USBDeviceCoreBestanden} )


