cmake_minimum_required(VERSION 3.7)

project(pointermap
        VERSION 0.0.0
        DESCRIPTION "Utility program for remapping 0xfffffff pointers in text files/streams to pointer_# form."
        LANGUAGES CXX
)

# set the use of C++17 globally as all examples require it
set(CMAKE_CXX_STANDARD 17)

# add clobber build target to clear all the non git registered files/directories
add_custom_target(clobber
    COMMAND git clean -d -f -x
)

set(SOURCES
    main.cpp
)

add_executable(pointermap ${SOURCES})

install(TARGETS pointermap
        RUNTIME DESTINATION bin
)

