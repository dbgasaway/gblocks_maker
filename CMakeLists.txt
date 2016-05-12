cmake_minimum_required (VERSION 3.3)

#creates static links if need be
macro(make_static)
    set(flags_configs "")
    if("${CMAKE_CONFIGURATION_TYPES}" STREQUAL "")
        # handle single config generators - like makefiles/ninja
        if("${CMAKE_BUILD_TYPE}" STREQUAL "")
            list(APPEND flags_configs CMAKE_C_FLAGS)
            list(APPEND flags_configs CMAKE_CXX_FLAGS)
        else()
            string(TOUPPER ${CMAKE_BUILD_TYPE} config)
            list(APPEND flags_configs CMAKE_C_FLAGS_${config})
            list(APPEND flags_configs CMAKE_CXX_FLAGS_${config})
        endif()
    else()
        # handle multi config generators (like msvc, xcode
        foreach(config ${CMAKE_CONFIGURATION_TYPES})
            string(TOUPPER ${config} config)
            list(APPEND flags_configs CMAKE_C_FLAGS_${config})
            list(APPEND flags_configs CMAKE_CXX_FLAGS_${config})
        endforeach()
    endif()

    #we current ignore gnu/clang, as they maybe don't need static linking
    foreach(flags ${flags_configs})
        if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
#             if(NOT ${flags} MATCHES "-static-libstdc\\+\\+")
#                 set(${flags} "${${flags}} -static-libstdc++")
#             endif()
#             if(NOT ${flags} MATCHES "-static-libgcc")
#                 set(${flags} "${${flags}} -static-libgcc")
#             endif()
			message(AUTHOR_WARNING "No static linking of GNU a.t.m.")
        elseif(MSVC)
            if(${flags} MATCHES "/MD")
                string(REGEX REPLACE "/MD" "/MT" ${flags} "${${flags}}")
            endif()
		else()
			message(AUTHOR_WARNING "Unknown compiler type - flags will be unchanged")
        endif()
    endforeach()
    
    list(FIND flags_configs "CMAKE_C_FLAGS" has_them)
    if(${has_them} STREQUAL "-1")
        list(APPEND flags_configs CMAKE_C_FLAGS)
        list(APPEND flags_configs CMAKE_CXX_FLAGS)
    endif()
endmacro()

project (Gblocks)
# The version number.
# set (Gblocks_VERSION_MAJOR 1)
# set (Gblocks_VERSION_MINOR 0)

#set(Boost_DEBUG 1)
#set(Boost_USE_STATIC_LIBS OFF)
#set(Boost_USE_MULTITHREADED ON)
#set(Boost_USE_STATIC_RUNTIME OFF)
#find_package(Boost REQUIRED COMPONENTS program_options)
#include_directories(${Boost_INCLUDE_DIRS})

make_static()

add_compile_options(-std=c++14)

add_executable(Gblocks main.cpp)
