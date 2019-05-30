cmake_minimum_required(VERSION 3.4.1)

add_library(png  SHARED
        ${CMAKE_CURRENT_SOURCE_DIR}/png.c
        ${CMAKE_CURRENT_SOURCE_DIR}/pngerror.c
        ${CMAKE_CURRENT_SOURCE_DIR}/pngget.c
        ${CMAKE_CURRENT_SOURCE_DIR}/pngmem.c
        ${CMAKE_CURRENT_SOURCE_DIR}/pngpread.c
        ${CMAKE_CURRENT_SOURCE_DIR}/pngread.c
        ${CMAKE_CURRENT_SOURCE_DIR}/pngrio.c
        ${CMAKE_CURRENT_SOURCE_DIR}/pngrtran.c
        ${CMAKE_CURRENT_SOURCE_DIR}/pngrutil.c
        ${CMAKE_CURRENT_SOURCE_DIR}/pngset.c
        ${CMAKE_CURRENT_SOURCE_DIR}/pngtest.c
        ${CMAKE_CURRENT_SOURCE_DIR}/pngtrans.c
        ${CMAKE_CURRENT_SOURCE_DIR}/pngwio.c
        ${CMAKE_CURRENT_SOURCE_DIR}/pngwrite.c
        ${CMAKE_CURRENT_SOURCE_DIR}/pngwtran.c
        ${CMAKE_CURRENT_SOURCE_DIR}/pngwutil.c
        ${CMAKE_CURRENT_SOURCE_DIR}/arm/arm_init.c
        ${CMAKE_CURRENT_SOURCE_DIR}/arm/filter_neon.S
        ${CMAKE_CURRENT_SOURCE_DIR}/arm/filter_neon_intrinsics.c
        ${CMAKE_CURRENT_SOURCE_DIR}/arm/palette_neon_intrinsics.c
        )

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/)

find_library( log-lib log )

target_link_libraries(png  ${log-lib} android EGL GLESv2  z)