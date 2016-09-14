FILE(GLOB F4StdPerifDriverBestanden src/*.c)
include_directories(./inc)

list(SORT F4StdPerifDriverBestanden)

if (0)

    macro(CONVERT_CYGWIN_PATH _path)
        EXECUTE_PROCESS(COMMAND cygpath.exe -aw ${${_path}}
                OUTPUT_VARIABLE ${_path})
        string(STRIP ${${_path}} ${_path})
    endmacro(CONVERT_CYGWIN_PATH)

    foreach (SRC_FILE ${F4StdPerifDriverBestanden})
        CONVERT_CYGWIN_PATH(SRC_FILE)
        set(CygF4StdPerifDriverBestanden ${CygF4StdPerifDriverBestanden} \"${SRC_FILE}\")
    endforeach ()
    add_library(F4StdPerifDriver ${CygF4StdPerifDriverBestanden})
else ()
    add_library(F4StdPerifDriver ${F4StdPerifDriverBestanden})
endif ()




