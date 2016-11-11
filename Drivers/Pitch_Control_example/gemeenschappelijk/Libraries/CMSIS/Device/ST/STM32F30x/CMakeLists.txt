
FILE (GLOB CoreSupport Source/Templates/*.c)
FILE (GLOB StartupCode Source/Templates/gcc_ride7/*.s)
#message(STATUS "startus=${StartupCode}")
add_library(CMSISFIRMWARE ${CoreSupport} ${StartupCode})
