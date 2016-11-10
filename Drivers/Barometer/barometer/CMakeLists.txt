## Copyright 2011-2016 Hogeschool van Arnhem en Nijmegen
## Opleiding Embedded Systems Engineering
## Ewout Boks
## $Id: CMakeLists.txt 2297 2016-09-08 10:49:19Z ewout $

message("HAN ESE STM32F0Discovery LEDs demo.")

set(PROJECTNAAM "barometer")

cmake_minimum_required(VERSION 2.8.10)

project(${PROJECTNAAM})

set(OverrideGCC_G++ ON)
add_definitions(-DHSE_VALUE=8000000)

set(CMAKESCRIPTDIR ${PROJECT_SOURCE_DIR}/../buildsystem)
include(${CMAKESCRIPTDIR}/STM32Development.cmake)
include_directories(h ${BASISDIR} ${F0DISCOVERYDIR})

include(${CMAKESCRIPTDIR}/STM32F0.cmake)

file(GLOB BasisBestanden ${BASISDIR}/stm32f0/*.c)
set(SRCS main.cpp usart.cpp MS5611.cpp ${BasisBestanden} stm32f0xx_it.c)

add_subdirectory(${F0DISCOVERYDIR} ${CMAKE_CURRENT_BINARY_DIR}/F0DiscoveryDir)

add_executable(${PROJECTNAAM}.elf ${SRCS})

# Doel wordt hier samengesteld.
target_link_libraries(${PROJECTNAAM}.elf STM32F0DiscoveryLib CMSISFIRMWARE F0StdPerifDriver)

PostBuildAktie(${PROJECTNAAM}.elf "STM32 ${PROJECTNAAM}.elf is gemaakt.")
