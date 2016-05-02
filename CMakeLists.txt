cmake_minimum_required (VERSION 3.3)

project (Gblocks)
# The version number.
# set (Gblocks_VERSION_MAJOR 1)
# set (Gblocks_VERSION_MINOR 0)

#set(Boost_DEBUG 1) 
set(Boost_USE_STATIC_LIBS OFF) 
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME OFF) 
find_package(Boost REQUIRED COMPONENTS program_options) 
include_directories(${Boost_INCLUDE_DIRS}) 

add_compile_options(-std=c++14)

add_executable(Gblocks main.cpp)