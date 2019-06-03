# CMakeLists.txt

# Copyright (C) 2018 Cosmin Truta
# Copyright (C) 2007,2009-2018 Glenn Randers-Pehrson
# Written by Christian Ehrlicher, 2007
# Revised by Roger Lowman, 2009-2010
# Revised by Clifford Yapp, 2011-2012,2017
# Revised by Roger Leigh, 2016
# Revised by Andreas Franek, 2016
# Revised by Sam Serrels, 2017
# Revised by Vadim Barkov, 2017
# Revised by Vicky Pfau, 2018
# Revised by Cameron Cawley, 2018
# Revised by Cosmin Truta, 2018
# Revised by Kyle Bentley, 2018

# This code is released under the libpng license.
# For conditions of distribution and use, see the disclaimer
# and license in png.h

cmake_minimum_required(VERSION 3.1)
cmake_policy(VERSION 3.1)

project(libpng C)


add_library(cpufeatures STATIC
        ${ANDROID_NDK}/sources/android/cpufeatures/cpu-features.c)

set(PNGLIB_MAJOR 1)
set(PNGLIB_MINOR 6)
set(PNGLIB_RELEASE 37)
set(PNGLIB_NAME libpng${PNGLIB_MAJOR}${PNGLIB_MINOR})
set(PNGLIB_VERSION ${PNGLIB_MAJOR}.${PNGLIB_MINOR}.${PNGLIB_RELEASE})


# COMMAND LINE OPTIONS
option(PNG_SHARED "Build shared lib" ON)
option(PNG_STATIC "Build static lib" OFF)

option(PNG_HARDWARE_OPTIMIZATIONS "Enable hardware optimizations" ON)

if (PNG_HARDWARE_OPTIMIZATIONS)

    # set definitions and sources for arm
    if (CMAKE_SYSTEM_PROCESSOR MATCHES "^arm" OR
            CMAKE_SYSTEM_PROCESSOR MATCHES "^aarch64")
        set(PNG_ARM_NEON_POSSIBLE_VALUES check on off)
        set(PNG_ARM_NEON "check" CACHE STRING "Enable ARM NEON optimizations:
     check: (default) use internal checking code;
     off: disable the optimizations;
     on: turn on unconditionally.")
        set_property(CACHE PNG_ARM_NEON PROPERTY STRINGS
                ${PNG_ARM_NEON_POSSIBLE_VALUES})
        list(FIND PNG_ARM_NEON_POSSIBLE_VALUES ${PNG_ARM_NEON} index)
        if (index EQUAL -1)
            message(FATAL_ERROR
                    "PNG_ARM_NEON must be one of [${PNG_ARM_NEON_POSSIBLE_VALUES}]")
        elseif (NOT ${PNG_ARM_NEON} STREQUAL "off")
            set(libpng_arm_sources
                    arm/arm_init.c
                    arm/filter_neon.S
                    arm/filter_neon_intrinsics.c
                    arm/palette_neon_intrinsics.c)

#            if (${PNG_ARM_NEON} STREQUAL "on")
                message("PNG_ARM_NEON is on")
                add_definitions(-DPNG_ARM_NEON_OPT=2)
#            elseif (${PNG_ARM_NEON} STREQUAL "check")
#                message("DPNG_ARM_NEON_CHECK_SUPPORTED is on")
#                add_definitions(-DPNG_ARM_NEON_CHECK_SUPPORTED)
#            endif ()
        else ()
            add_definitions(-DPNG_ARM_NEON_OPT=0)
        endif ()
    endif ()

else (PNG_HARDWARE_OPTIMIZATIONS)

    # set definitions and sources for arm
    if (CMAKE_SYSTEM_PROCESSOR MATCHES "^arm" OR
            CMAKE_SYSTEM_PROCESSOR MATCHES "^aarch64")
        add_definitions(-DPNG_ARM_NEON_OPT=0)
    endif ()


endif (PNG_HARDWARE_OPTIMIZATIONS)

set_property(SOURCE ${libpng_arm_sources}
        APPEND_STRING PROPERTY COMPILE_FLAGS " -mfpu=neon")


# SET LIBNAME
set(PNG_LIB_NAME png)

if (NOT AWK OR ANDROID)
    # No awk available to generate sources; use pre-built pnglibconf.h
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/scripts/pnglibconf.h.prebuilt
            ${CMAKE_CURRENT_SOURCE_DIR}/pnglibconf.h)
    add_custom_target(genfiles) # Dummy
endif ()


include_directories(${CMAKE_CURRENT_BINARY_DIR})


# OUR SOURCES
set(libpng_public_hdrs
        png.h
        pngconf.h
          "${CMAKE_CURRENT_SOURCE_DIR}/pnglibconf.h"
        )
set(libpng_private_hdrs
        pngpriv.h
        pngdebug.h
        pnginfo.h
        pngstruct.h
        )

set(libpng_sources
        ${libpng_public_hdrs}
        ${libpng_private_hdrs}
        png.c
        pngerror.c
        pngget.c
        pngmem.c
        pngpread.c
        pngread.c
        pngrio.c
        pngrtran.c
        pngrutil.c
        pngset.c
        pngtrans.c
        pngwio.c
        pngwrite.c
        pngwtran.c
        pngwutil.c
        ${libpng_arm_sources}
        )

find_library( # Sets the name of the path variable.
        log-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        log)



if (PNG_SHARED)
    message("build png shared library")
    add_library(png SHARED ${libpng_sources})
    add_dependencies(png genfiles)
#    set_target_properties(png PROPERTIES OUTPUT_NAME ${PNG_LIB_NAME})
    target_link_libraries(png z log cpufeatures)
    target_include_directories(png PRIVATE
            ${ANDROID_NDK}/sources/android/cpufeatures)
endif ()

if (PNG_STATIC)
    # does not work without changing name
    message("build png static library")
    set(PNG_LIB_NAME_STATIC png_static)
    add_library(png_static STATIC ${libpng_sources})
    # MSVC doesn't use a different file extension for shared vs. static
    # libs.  We are able to change OUTPUT_NAME to remove the _static
    # for all other platforms.
    if (NOT MSVC)
        set_target_properties(png_static PROPERTIES
                OUTPUT_NAME "${PNG_LIB_NAME}_static"
                CLEAN_DIRECT_OUTPUT 1)
    else ()
        set_target_properties(png_static PROPERTIES
                OUTPUT_NAME "${PNG_LIB_NAME}_static"
                CLEAN_DIRECT_OUTPUT 1)
    endif ()
    target_link_libraries(png_static z)
endif ()

