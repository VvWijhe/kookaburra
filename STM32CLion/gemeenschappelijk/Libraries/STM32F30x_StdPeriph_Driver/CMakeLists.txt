
FILE (GLOB F30StdPerifDriverBestanden src/*.c)

list(SORT F30StdPerifDriverBestanden)

include_directories(./inc)

add_library(F30StdPerifDriver ${F30StdPerifDriverBestanden} )


