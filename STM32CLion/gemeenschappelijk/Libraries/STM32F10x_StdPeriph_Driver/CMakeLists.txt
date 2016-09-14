## Copyright 2011-2016 Hogeschool van Arnhem en Nijmegen
## $Id: CMakeLists.txt 2240 2016-06-09 19:21:07Z ewout $

FILE(GLOB F1StdPerifDriverBestanden src/*.c)

list(SORT F1StdPerifDriverBestanden)

include_directories(inc)

MaakLib(F1StdPerifDriver "${F1StdPerifDriverBestanden}")
