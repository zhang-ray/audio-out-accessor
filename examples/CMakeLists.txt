
set (CMAKE_CXX_STANDARD 11)

include_directories(../include/)
include_directories(../src/)

add_executable(01.oscilloscope 01.oscilloscope.cpp)
target_link_libraries(01.oscilloscope AOA pulse SDL2)
