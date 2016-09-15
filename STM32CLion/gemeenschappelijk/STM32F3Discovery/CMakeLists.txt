# ST package voor STM32F4 Discovery board
set(Bronnen stm32f3_discovery.c stm32f3Controller.cpp stm32f3Perifs.cpp)

#message(STATUS "bronnen=${Bronnen}")
include_directories(.)

add_library(STMF32F3DiscoveryLib ${Bronnen})

