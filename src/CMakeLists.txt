set(SRC loadavg.c)

if (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set(SRC ${SRC}
            darwin/mem.c)
endif ()

if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(SRC ${SRC}
            linux/mem.c)
endif ()

add_library(hoststat SHARED ${SRC})

install (FILES ../include/hoststat.h DESTINATION include)
install (TARGETS hoststat DESTINATION lib)