## Copyright 2011-2016 Hogeschool van Arnhem en Nijmegen
## $Id: CMakeLists.txt 2239 2016-06-09 19:20:30Z ewout $

set(DeviceSupportDir DeviceSupport/ST/STM32F10x)
file(GLOB CM3CoreSupport CoreSupport/*.c)

file(GLOB CM3DeviceSupport ${DeviceSupportDir}/*.c)

set(StartupBestand ${DeviceSupportDir}/startup/gcc/startup_stm32f10x_md.c)

#message(STATUS "cortex-m3 coresupport=${CM3CoreSupport}")

set(SRCS ${CM3CoreSupport} ${CM3DeviceSupport} ${StartupBestand})

list(SORT SRCS)
MaakLib(CMSISFIRMWARE "${SRCS}")
