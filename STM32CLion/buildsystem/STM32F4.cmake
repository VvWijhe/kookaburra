#************************************************************************
# CMake script om STM32 F4 software te compileren.
# Copyright 2004-2014 Hogeschool van Arnhem en Nijmegen
# Opleiding Embedded Systems Engineering
# $Id: STM32F4.cmake 2279 2016-08-29 15:00:23Z ewout $
#************************************************************************

message(STATUS "Bouw target voor STM32F40x met 1 MByte Flash en 192K SRAM")

include(${CMAKESCRIPTDIR}/BasisSTM32.cmake)

add_definitions(-DARMCORTEXM4)

set(ARMCORTEXM4F ON)

set(stm32f4cmsisdir ${STLIBRARIES}/CMSIS/Device/ST/STM32F4xx)
set(stm32f4perifdir ${STLIBRARIES}/STM32F4xx_StdPeriph_Driver)
set(stm32firmwareinclude ${STLIBRARIES}/CMSIS/Include ${stm32f4cmsisdir}/Include ${stm32f4perifdir}/inc)
set(stm32linkermap ${STLIBRARIES}/ldscripts/STM32F40x_1024k_192k_flash.ld)


set(CMAKE_ASM-ATT_FLAGS ${CMAKE_ASM_CM4VLAGGEN})
set(CMAKE_C_FLAGS "${CMAKE_C_CM4FVLAGGEN}")
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_CM4FVLAGGEN})
set(CMAKE_C_LINK_FLAGS "${CMAKE_C_CM4FVLAGGEN} -T ${stm32linkermap} -Wl,-Map,${PROJECTNAAM}.map -nostartfiles")
#   -Xlinker --defsym=_isatty=_isatty_r
set(CMAKE_CXX_LINK_FLAGS ${CMAKE_C_LINK_FLAGS})

include_directories(${stm32firmwareinclude})

add_subdirectory(${stm32f4cmsisdir} ${CMAKE_CURRENT_BINARY_DIR}/STM32CMSIS)
add_subdirectory(${stm32f4perifdir} ${CMAKE_CURRENT_BINARY_DIR}/STM32_StdPeriph_Driver)

if (USE_USBDEVICE)

    include_directories(${STLIBRARIES}/STM32_USB_OTG_Driver/inc)
    message(STATUS "STM32 USB OTG Device driver will be included")
    add_subdirectory(${STLIBRARIES}/STM32_USB_OTG_Driver ${CMAKE_CURRENT_BINARY_DIR}/STM32_USB_OTG_Driver)

    message(STATUS "STM32 USB Device Library will be included")
    add_subdirectory(${STLIBRARIES}/STM32_USB_Device_Library ${CMAKE_CURRENT_BINARY_DIR}/STM32_USB_Device_Library)
    include_directories(${STLIBRARIES}/STM32_USB_Device_Library/Core/inc)
endif ()

if (USE_I2C_CPAL)
    message(STATUS "STM32F4xx I2C CPAL v1 driver will be included")
    include_directories(${STLIBRARIES}/STM32_CPALv1_Driver/inc)
    add_subdirectory(${STLIBRARIES}/STM32_CPALv1_Driver ${CMAKE_CURRENT_BINARY_DIR}/STM32_CPALv1_Driver)
endif ()






