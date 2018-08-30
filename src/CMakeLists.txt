
set (CMAKE_CXX_STANDARD 11)

include_directories(../include/)

file(GLOB_RECURSE FILES_SOURCES *.cpp *.c)
file(GLOB_RECURSE FILES_INCLUDE *.hpp *.h ../include/*.hpp ../include/*.h)


add_library(${PROJECT_NAME} SHARED ${FILES_INCLUDE} ${FILES_SOURCES})
