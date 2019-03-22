cmake_minimum_required(VERSION 3.5)

project(Challenge)

set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z -Wall -Wextra -Werror -D_FILE_OFFSET_BITS=64")

add_executable(Challenge Main.cpp AVI/AVIHeader.cpp AVI/AVISizes.cpp AVI/AVIStruct.h ConvertImage/ConversionHelper.cpp 
ConvertImage/ConversionHelper.h)
