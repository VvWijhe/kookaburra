
FILE(GLOB SRCS src/*.c)

list(SORT SRCS)

include_directories(./inc)

# tijdelijk voor debugging
#add_definitions(-O0)

add_library(STM32F0CPALDriver ${SRCS})


