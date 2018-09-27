
cmake_minimum_required(VERSION 3.4.1)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.


add_library( # Sets the name of the library.
             opengl
             # Sets the library as a shared library.
             SHARED
             # Provides a relative path to your source file(s).
             ${CMAKE_CURRENT_SOURCE_DIR}/opengl_util.cpp
             ${CMAKE_CURRENT_SOURCE_DIR}/egl_core.cpp
             ${CMAKE_CURRENT_SOURCE_DIR}/egl_surface_base.cpp
             ${CMAKE_CURRENT_SOURCE_DIR}/window_surface.cpp
             ${CMAKE_CURRENT_SOURCE_DIR}/offscreen_surface.cpp
             )

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
              log-lib

              # Specifies the name of the NDK library that
              # you want CMake to locate.
              log )


# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.


target_link_libraries( # Specifies the target library.
                       opengl
                       GLESv2
                       EGL
                       android
                       # Links the target library to the log library
                       ${log-lib} )