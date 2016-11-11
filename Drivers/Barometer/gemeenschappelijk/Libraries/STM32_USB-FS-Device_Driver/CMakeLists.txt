include_directories(./inc)

FILE(GLOB SRCS src/*.c src/*.cpp)

add_library(STM32USBFSDriver ${CUSTOMUSBDEEL} ${SRCS})


