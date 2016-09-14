## (c)2011-2013 Ewout Boks
## Hogeschool van Arnhem en Nijmegen
## Opleiding Embedded Systems Engineering
# $Id: CMakeLists.txt 2279 2016-08-29 15:00:23Z ewout $

message("HAN ESE STM32F3 demo.")
message("Copyright 2011-2013 ir drs E.J Boks")
message("Opleiding Embedded Systems Engineering")
message("Hogeschool van Arnhem en Nijmegen")

set(PROJECTNAAM "stm32f3demo")

cmake_minimum_required(VERSION 2.8.4)

project(${PROJECTNAAM})

set(OverrideGCC_G++ ON)
add_definitions(-DHSE_VALUE=8000000)

set(CMAKESCRIPTDIR ${PROJECT_SOURCE_DIR}/../buildsystem)
include(${CMAKESCRIPTDIR}/STM32Development.cmake)
include_directories(. ${BASISDIR} ${F3DISCOVERYDIR} h)

include(${CMAKESCRIPTDIR}/STM32F30.cmake)

set(GEMDIR ${PROJECT_SOURCE_DIR}/../gemeenschappelijk)
set(DISCOVERYDIR ${GEMDIR}/STM32F3Discovery)

set(SRCS main.cpp leds.cpp syscalls.c term_io.c comm.c)

add_subdirectory(${DISCOVERYDIR} ${CMAKE_CURRENT_BINARY_DIR}/DiscoveryDir)

add_executable(${PROJECTNAAM}.elf ${SRCS})

# Doel wordt hier samengesteld.
target_link_libraries(${PROJECTNAAM}.elf STMF32F3DiscoveryLib CMSISFIRMWARE F30StdPerifDriver)

PostBuildAktie(${PROJECTNAAM}.elf "STM32 ${PROJECTNAAM}.elf is gemaakt.")
