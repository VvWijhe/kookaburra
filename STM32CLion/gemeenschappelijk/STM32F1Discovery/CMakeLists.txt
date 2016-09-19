## Copyright 2011-2016 Hogeschool van Arnhem en Nijmegen
## $Id: CMakeLists.txt 2241 2016-06-09 19:21:18Z ewout $

set(SRCS STM32vldiscovery.c stm32Periphs.cpp stm32Controller.cpp algdef.cpp)
#Retarget.c)

list(SORT SRCS)

MaakLib(STM32F1DISCOVERY "${SRCS}")


