## Copyright 2011-2014 Hogeschool van Arnhem en Nijmegen
## Opleiding Embedded Systems Engineering
## Ewout Boks
## $Id: CMakeLists.txt 1779 2014-12-10 15:55:04Z ewout $

message("HAN ESE STM32F4 LEDs demo.")
message("(c)2011-2013 ir drs E.J Boks")
message("Opleiding Embedded Systems Engineering")
message("Hogeschool van Arnhem en Nijmegen")

set(PROJECTNAAM "stm32f4leddemo")

cmake_minimum_required(VERSION 2.8.4)

project(${PROJECTNAAM})

set(GEMDIR ${PROJECT_SOURCE_DIR}/../gemeenschappelijk)
set(DISCOVERYDIR ${GEMDIR}/STM32F4Discovery)
set(BASISDIR ${GEMDIR}/basis)
set(CMAKESCRIPTDIR ${PROJECT_SOURCE_DIR}/../../buildsystem/STM32)

# Discovery heeft 8 MHz 
add_definitions(-DUSE_STDPERIPH_DRIVER -DHSE_VALUE=8000000)

include_directories(h ${DISCOVERYDIR})

include(${CMAKESCRIPTDIR}/STM32F4.cmake)
set(SRCS main.cpp leds.cpp syscalls.c term_io.c comm.c)

add_subdirectory(${DISCOVERYDIR} ${CMAKE_CURRENT_BINARY_DIR}/DiscoveryDir)

add_executable(${PROJECTNAAM}.elf ${SRCS})

# voor info na het linken
add_custom_target(info ALL
        COMMAND ${COMP_INFO_TOOL} -A -d ${PROJECTNAAM}.elf
        DEPENDS ${PROJECTNAAM}.elf
        COMMENT "STM32 ${PROJECTNAAM}.elf is gemaakt. Hier volgt informatie over het ELF bestand.")

# Doel wordt hier samengesteld.
target_link_libraries(${PROJECTNAAM}.elf STMF32F4DiscoveryLib CMSISFIRMWARE F4StdPerifDriver)
