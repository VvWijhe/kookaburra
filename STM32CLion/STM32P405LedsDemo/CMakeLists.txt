# Copyright 2004-2014 ir drs E.J Boks
# Hogeschool van Arnhem en Nijmegen
# Opleiding Embedded Systems Engineering
# $Id: CMakeLists.txt 2279 2016-08-29 15:00:23Z ewout $

message("HAN ESE Olimex STM32P405 LEDs demo.")
message("(c)2011-2014 ir drs E.J Boks")
message("Opleiding Embedded Systems Engineering")
message("Hogeschool van Arnhem en Nijmegen")

set(PROJECTNAAM "stm32p405demo")

cmake_minimum_required(VERSION 2.8.4)

project(${PROJECTNAAM})

set(OverrideGCC_G++ ON)
set(CMAKESCRIPTDIR ${PROJECT_SOURCE_DIR}/../buildsystem)
include(${CMAKESCRIPTDIR}/STM32Development.cmake)

# Olimex P405 board heeft 8 MHz crystal
add_definitions(-DUSE_STDPERIPH_DRIVER -DHSE_VALUE=8000000)

include(${CMAKESCRIPTDIR}/STM32F4.cmake)

set(SRCS main.cpp leds.cpp syscalls.c term_io.c comm.c)

add_executable(${PROJECTNAAM}.elf ${SRCS})

# Doel wordt hier samengesteld.
target_link_libraries(${PROJECTNAAM}.elf F4StdPerifDriver CMSISFIRMWARE)

PostBuildAktie(${PROJECTNAAM}.elf "STM32 ${PROJECTNAAM}.elf is gemaakt.")

