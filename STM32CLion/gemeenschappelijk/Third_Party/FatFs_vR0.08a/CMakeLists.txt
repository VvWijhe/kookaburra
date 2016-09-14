
FILE(GLOB FatFSBestanden *.c)
include_directories(.)

message(STATUS "FatFS bibliotheek wordt gecompileerd.")
add_library(FatFSLib ${FatFSBestanden})
