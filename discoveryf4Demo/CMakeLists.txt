## Copyright 2011-2016 Hogeschool van Arnhem en Nijmegen
## Opleiding Embedded Systems Engineering
## Ewout Boks
## $Id: CMakeLists.txt 2279 2016-08-29 15:00:23Z ewout $
 
message("HAN ESE STM32F4 LEDs demo.")
message("(c)2011-2013 ir drs E.J Boks")
message("Opleiding Embedded Systems Engineering")
message("Hogeschool van Arnhem en Nijmegen")

set(PROJECTNAAM "stm32f4discoverdemo")

cmake_minimum_required(VERSION 2.8.4)

project(${PROJECTNAAM})

set(OverrideGCC_G++ ON)
set(CMAKESCRIPTDIR ${PROJECT_SOURCE_DIR}/../buildsystem)
include(${CMAKESCRIPTDIR}/STM32Development.cmake)

# Discovery heeft 8 MHz 
add_definitions( -DUSE_STDPERIPH_DRIVER -DHSE_VALUE=8000000)

include_directories(h ${F4DISCOVERYDIR})

include(${CMAKESCRIPTDIR}/STM32F4.cmake)

set(SRCS main.cpp leds.cpp syscalls.c term_io.c comm.c)

add_subdirectory(${F4DISCOVERYDIR} ${CMAKE_CURRENT_BINARY_DIR}/F4DiscoveryDir)

add_executable(${PROJECTNAAM}.elf ${SRCS})
				   
# Doel wordt hier samengesteld.
target_link_libraries(${PROJECTNAAM}.elf STMF32F4DiscoveryLib CMSISFIRMWARE  F4StdPerifDriver)

PostBuildAktie(${PROJECTNAAM}.elf "STM32 ${PROJECTNAAM}.elf is gemaakt.")
