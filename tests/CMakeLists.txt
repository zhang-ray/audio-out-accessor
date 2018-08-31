
set (CMAKE_CXX_STANDARD 11)

include_directories(../include/)
include_directories(../src/)

add_executable(test01 test01.cpp)
target_link_libraries(test01 AOA pulse)
