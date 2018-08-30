
set (CMAKE_CXX_STANDARD 11)

include_directories(../include/)
include_directories(../src/)

add_executable(example00 example00.cpp)

target_link_libraries(example00 AOA pulse)
