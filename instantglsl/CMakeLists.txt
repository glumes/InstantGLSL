# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

ADD_SUBDIRECTORY(${CMAKE_SOURCE_DIR}/src/main/cpp/drawer/)
ADD_SUBDIRECTORY(${CMAKE_SOURCE_DIR}/src/main/cpp/opengl/)
ADD_SUBDIRECTORY(${CMAKE_SOURCE_DIR}/src/main/cpp/filemap/)
ADD_SUBDIRECTORY(${CMAKE_SOURCE_DIR}/src/main/cpp/camerahelper/)
ADD_SUBDIRECTORY(${CMAKE_SOURCE_DIR}/src/main/cpp/glm)
ADD_SUBDIRECTORY(${CMAKE_SOURCE_DIR}/src/main/cpp/libpng)
ADD_SUBDIRECTORY(${CMAKE_SOURCE_DIR}/src/main/cpp/pnghelper)

#ADD_SUBDIRECTORY(${CMAKE_SOURCE_DIR}/src/main/cpp/jpeghelper)



include_directories(${CMAKE_SOURCE_DIR}/src/main/cpp/drawer/)
include_directories(${CMAKE_SOURCE_DIR}/src/main/cpp/opengl/)
include_directories(${CMAKE_SOURCE_DIR}/src/main/cpp/filemap/)
include_directories(${CMAKE_SOURCE_DIR}/src/main/cpp/stb/)
include_directories(${CMAKE_SOURCE_DIR}/src/main/cpp/camerahelper/)
include_directories(${CMAKE_SOURCE_DIR}/src/main/cpp/glm/)
include_directories(${CMAKE_SOURCE_DIR}/src/main/cpp/libpng/)
include_directories(${CMAKE_SOURCE_DIR}/src/main/cpp/pnghelper/)

include_directories(${CMAKE_SOURCE_DIR}/src/main/cpp/libjpeg/)
include_directories(${CMAKE_SOURCE_DIR}/src/main/cpp/jpeghelper/)

include_directories(${CMAKE_SOURCE_DIR}/src/main/cpp/base/)


add_library( # Sets the name of the library.
        instantglsl

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        src/main/cpp/instant-renderer.cpp
        ${CMAKE_SOURCE_DIR}/src/main/cpp/base/Log.cpp
        )

add_library( # Sets the name of the library.
        cameraglsl

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        src/main/cpp/camera-renderer.cpp
        ${CMAKE_SOURCE_DIR}/src/main/cpp/base/Log.cpp
        )


# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
        log-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        log)

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.



add_library(libturbojpeg SHARED IMPORTED)
set_target_properties(libturbojpeg PROPERTIES IMPORTED_LOCATION
        ${CMAKE_SOURCE_DIR}/src/main/jniLibs/armeabi-v7a/libturbojpeg.so)

add_library(libjpeg SHARED IMPORTED)
set_target_properties(libjpeg PROPERTIES IMPORTED_LOCATION
        ${CMAKE_SOURCE_DIR}/src/main/jniLibs/armeabi-v7a/libjpeg.so)


add_library( # Sets the name of the library.
        jpeghelper
        # Sets the library as a shared library.
        SHARED
        # Provides a relative path to your source file(s).
        ${CMAKE_SOURCE_DIR}/src/main/cpp/jpeghelper/JpegHelper.cpp
        ${CMAKE_SOURCE_DIR}/src/main/cpp/base/Log.cpp
        )


target_link_libraries( # Specifies the target library.
        jpeghelper
        android
        libturbojpeg
        libjpeg
        # Links the target library to the log library
        ${log-lib})



target_link_libraries( # Specifies the target library.
        cameraglsl
        camerahelper
        opengl
        drawer
        filemap
        android
        glm_shared
        png
        pnghelper
        # Links the target library to the log library
        # included in the NDK.
        ${log-lib})


target_link_libraries( # Specifies the target library.
        instantglsl
        opengl
        drawer
        filemap
        android
        png
        pnghelper
        # Links the target library to the log library
        # included in the NDK.
        ${log-lib})