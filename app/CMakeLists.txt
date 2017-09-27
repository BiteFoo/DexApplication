# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

add_library( # Sets the name of the library.
             native-lib

             # Sets the library as a shared library.
             SHARED

             # Provides a relative path to your source file(s).

             #change code
             #src/main/cpp/mandroid/CommUtils.cpp
             #src/main/cpp/mandroid/myjni.cpp

             #**********change code by method
             src/main/cpp/native-lib.cpp
             src/main/cpp/mandroid/MDalvik.cpp

             #*********change code by DexFile
             #src/main/cpp/Android/libdex/DexFile.cpp
             #src/main/cpp/Android/libdex/CmdUtils.cpp
             #src/main/cpp/Android/libdex/DexCatch.cpp
             #src/main/cpp/Android/libdex/DexClass.cpp
             # src/main/cpp/Android/libdex/DexDataMap.cpp
               #src/main/cpp/Android/libdex/DexDebugInfo.cpp
             # src/main/cpp/Android/libdex/DexInlines.cpp
              # src/main/cpp/Android/libdex/DexOpcodes.cpp
              # src/main/cpp/Android/libdex/DexOptData.cpp
             #  src/main/cpp/Android/libdex/DexProto.cpp
              # src/main/cpp/Android/libdex/DexSwapVerify.cpp
             #  src/main/cpp/Android/libdex/DexUtf.cpp
             #  src/main/cpp/Android/libdex/InstrUtils.cpp
            #   src/main/cpp/Android/libdex/Leb128.cpp
             #  src/main/cpp/Android/libdex/OptInvocation.cpp
            #   src/main/cpp/Android/libdex/sha1.cpp
            #   src/main/cpp/Android/libdex/SysUtil.cpp
             #  src/main/cpp/Android/libdex/ZipArchive.cpp
               #****************************
            #   src/main/cpp/Android/vm/oo/Class.cpp
            #   src/main/cpp/Android/vm/oo/Object.cpp
             #  src/main/cpp/Android/vm/oo/Resolve.cpp
               #********************
             #   src/main/cpp/Android/vm/DvmDex.cpp
             #    src/main/cpp/Android/vm/LinearAlloc.cpp
             #    src/main/cpp/Android/Extra.cpp
             )

include_directories(
src/main/cpp/mandroid
#src/main/cpp/Android/libdex
#src/main/cpp/Android/vm/oo
src/main/cpp/Android

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
              log )

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
                       native-lib

                       # Links the target library to the log library
                       # included in the NDK.
                       ${log-lib} )