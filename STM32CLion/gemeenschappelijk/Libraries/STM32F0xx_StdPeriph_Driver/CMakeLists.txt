message(STATUS "STM32F0xx Perif Driver wordt zonder debug ondersteuning gecompileerd")
add_definitions(-Os -g0 -DNDEBUG)

FILE (GLOB F0StdPerifDriverBestanden src/*.c)

list(SORT F0StdPerifDriverBestanden)

include_directories(./inc)

add_library(F0StdPerifDriver ${F0StdPerifDriverBestanden} )

