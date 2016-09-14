
FILE(GLOB CPALBestanden *.c)

include_directories(.)

add_library(STM32F4xxCPALv1Driver ${CPALBestanden})


 