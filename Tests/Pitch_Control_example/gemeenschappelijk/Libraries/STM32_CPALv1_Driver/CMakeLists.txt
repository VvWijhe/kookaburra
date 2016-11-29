
set(CPALDEVICE devices/${I2C_CPAL_DEVICE})

FILE (GLOB STM32CPALv1DriverBestanden ${CPALDEVICE}/*.c src/*.c)

include_directories(./inc ${CPALDEVICE})

add_library(STM32CPALv1Driver ${STM32CPALv1DriverBestanden} )


