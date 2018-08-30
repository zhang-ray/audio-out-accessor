
set (CMAKE_CXX_STANDARD 11)

include_directories(../include/)
include_directories(../src/)

add_executable(test00 test00.cpp)
target_link_libraries(test00 AOA pulse)
