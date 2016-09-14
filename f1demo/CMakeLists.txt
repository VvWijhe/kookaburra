## (c)2011-2012 Ewout Boks
## Hogeschool van Arnhem en Nijmegen
## Opleiding Embedded Systems Engineering
 
message("HAN ESE STM32F103 demo.")

set(PROJECTNAAM "stm32f1demo")

cmake_minimum_required(VERSION 2.8.4)

project(${PROJECTNAAM})

set(OverrideGCC_G++ ON)
add_definitions(-DHSE_VALUE=8000000)

set(CMAKESCRIPTDIR ${PROJECT_SOURCE_DIR}/../buildsystem)
include(${CMAKESCRIPTDIR}/STM32Development.cmake)
include_directories(. ${BASISDIR} ${F1DISCOVERYDIR})

add_definitions(-DSTM32F10X_MD -DUSE_STM3210E_EVAL -DUSE_STDPERIPH_DRIVER)

include_directories(. )

set(STM32F1_128K_20K off)
set(STM32F1_512K_64K on)
set(STM32F2_1MByte off)
set(STM32F4_1MByte off)

include(${CMAKESCRIPTDIR}/STM32F1.cmake)

set(SRCS comm.c main.cpp mini_cpp.cpp myclasses.cpp startup_stm32f10x_md_mthomas.c stm32f10x_it.c syscalls.c term_io.c timebase.c)

add_executable(${PROJECTNAAM}.elf ${SRCS})

# Doel wordt hier samengesteld.
target_link_libraries(${PROJECTNAAM}.elf F1StdPerifDriver CMSISFIRMWARE)

PostBuildAktie(${PROJECTNAAM}.elf "STM32 ${PROJECTNAAM}.elf is gemaakt. Hier volgt informatie over het ELF bestand.")


