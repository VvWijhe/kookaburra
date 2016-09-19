#************************************************************************
# CMake script om STM32 F0 software te compileren.
# Copyright 2004-2014 Hogeschool van Arnhem en Nijmegen
# Opleiding Embedded Systems Engineering
# $Id: STM32F0.cmake 2279 2016-08-29 15:00:23Z ewout $
#************************************************************************
 
message(STATUS "Bouw target voor STM32F051R8T6 met 64 KByte Flash en 8K SRAM")

add_definitions(-DARMCORTEXM0)
add_definitions(-Darm -Dstm32 -Dstm32f0xx -DSTM32F0XX)

include(${CMAKESCRIPTDIR}/BasisSTM32.cmake)

set(stm32f0cmsisdir ${STLIBRARIES}/CMSIS/Device/ST/STM32F0xx)
set(stm32f0perifdir ${STLIBRARIES}/STM32F0xx_StdPeriph_Driver)
set(stm32firmwareinclude  ${STLIBRARIES}/CMSIS/Include ${stm32f0cmsisdir}/Include ${stm32f0perifdir}/inc)

set(stm32linkermap ${STLIBRARIES}/ldscripts/STM32F0x_64k_8k_flash.ld)

set (CMAKE_ASM-ATT_FLAGS ${CMAKE_ASM_CM0VLAGGEN})
set (CMAKE_C_FLAGS "${CMAKE_C_CM0VLAGGEN} ")
set (CMAKE_CXX_FLAGS ${CMAKE_CXX_CM0VLAGGEN})
set (CMAKE_C_LINK_FLAGS "${StandaardLinkVlaggen} ${CMAKE_C_CM0VLAGGEN} -T ${stm32linkermap} -nostartfiles")
set(CMAKE_CXX_LINK_FLAGS ${CMAKE_C_LINK_FLAGS})

#  -Xlinker --defsym=_isatty=_isatty_r

include_directories(${stm32firmwareinclude})

add_subdirectory(${stm32f0cmsisdir} ${CMAKE_CURRENT_BINARY_DIR}/STM32CMSIS)
add_subdirectory(${stm32f0perifdir} ${CMAKE_CURRENT_BINARY_DIR}/STM32_StdPeriph_Driver)

if (USE_I2C_CPAL)
set(CPALDIRNAAM STM32F0xx_CPAL_Driver)
 message(STATUS "STM32F0xx I2C CPAL v2 driver will be included")
 include_directories(${STLIBRARIES}/${CPALDIRNAAM}/inc)
 add_subdirectory(${STLIBRARIES}/${CPALDIRNAAM} ${CMAKE_CURRENT_BINARY_DIR}/${CPALDIRNAAM})
endif()

