# ST package voor STM32F4 Discovery board
set(Bronnen stm32f4_discovery.c stm32f4Controller.cpp stm32f4Perifs.cpp)

#message(STATUS "bronnen=${Bronnen}")
include_directories(.)

add_library(STMF32F4DiscoveryLib ${Bronnen} )

