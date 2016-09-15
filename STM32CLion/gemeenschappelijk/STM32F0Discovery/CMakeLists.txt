# ST package voor STM32F0 Discovery board
set(Bronnen stm32f0_discovery.c)
include_directories(.)
add_library(STM32F0DiscoveryLib ${Bronnen})
