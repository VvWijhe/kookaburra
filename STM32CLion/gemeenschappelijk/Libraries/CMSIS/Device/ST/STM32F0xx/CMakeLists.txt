
FILE(GLOB CoreSupport Source/Templates/*.c)
FILE(GLOB StartupCode Source/Templates/gcc/*.s)
#message(STATUS "startus=${StartupCode}")
add_library(CMSISFIRMWARE ${CoreSupport} ${StartupCode})
