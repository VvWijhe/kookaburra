## Copyright 2011-2014 Hogeschool van Arnhem en Nijmegen
## Opleiding Embedded Systems Engineering
## Ewout Boks
## $Id: STM32F30.cmake 2279 2016-08-29 15:00:23Z ewout $

message(STATUS "Bouw target voor STM32F30x met 256 KByte Flash en 40K SRAM")

include(${CMAKESCRIPTDIR}/BasisSTM32.cmake)

add_definitions(-DARMCORTEXM4)

set(stm32f3cmsisdir ${STLIBRARIES}/CMSIS/Device/ST/STM32F30x)
set(stm32f3perifdir ${STLIBRARIES}/STM32F30x_StdPeriph_Driver)
set(stm32firmwareinclude ${STLIBRARIES}/CMSIS/Include ${stm32f3cmsisdir}/Include ${stm32f3perifdir}/inc)
set(stm32linkermap ${STLIBRARIES}/ldscripts/STM32F30x_256k_40k_flash.ld)

set(CMAKE_ASM-ATT_FLAGS ${CMAKE_ASM_CM4VLAGGEN})
set(CMAKE_C_FLAGS "${CMAKE_C_CM4FVLAGGEN}")
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_CM4FVLAGGEN})
set(CMAKE_C_LINK_FLAGS "${CMAKE_C_CM4FVLAGGEN} -T ${stm32linkermap} -Wl,-Map,${PROJECTNAAM}.map -nostartfiles")
#   -Xlinker --defsym=_isatty=_isatty_r
set(CMAKE_CXX_LINK_FLAGS ${CMAKE_C_LINK_FLAGS})

include_directories(${stm32firmwareinclude})

add_subdirectory(${stm32f3cmsisdir} ${CMAKE_CURRENT_BINARY_DIR}/STM32CMSIS)
add_subdirectory(${stm32f3perifdir} ${CMAKE_CURRENT_BINARY_DIR}/STM32_StdPeriph_Driver)

if (USE_USBDEVICE)

    message(STATUS "STM32 USB FS Device driver will be included")
    include_directories(${STLIBRARIES}/STM32_USB-FS-Device_Driver/inc)
    add_subdirectory(${STLIBRARIES}/STM32_USB-FS-Device_Driver ${CMAKE_CURRENT_BINARY_DIR}/STM32_USB-FS-Device_Driver)
endif ()

if (USE_I2C_CPAL)
    message(STATUS "STM32F30x I2C CPAL v2 driver will be included")
    include_directories(${STLIBRARIES}/STM32_CPALv2_Driver/inc)
    add_subdirectory(${STLIBRARIES}/STM32_CPALv2_Driver ${CMAKE_CURRENT_BINARY_DIR}/STM32_CPALv2_Driver)
endif ()



