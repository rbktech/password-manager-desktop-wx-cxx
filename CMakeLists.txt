cmake_minimum_required(VERSION 3.10)

file(STRINGS "version" VERSION)

project(CodeManager VERSION ${VERSION})

#set(ROOT "C:/PasswordManager")
#set(RESOURCES "${ROOT}/resources")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_VERBOSE_MAKEFILE OFF)
#set(STATIC_JDK_LINK OFF)

set(wxWidgets_LIB_DIR $ENV{WXWIN}/lib/gcc_libx64)
find_package(wxWidgets 3.2 REQUIRED core base xml)

add_definitions(-DVERSION="${PROJECT_VERSION}")
#add_definitions(-DJDK="$ENV{JDK}")
#add_definitions(-DRESOURCES="${RESOURCES}")

#if(STATIC_JDK_LINK MATCHES ON)

#    add_definitions(-DSTATIC_JDK_LINK)

#    set(OUTPUT_DIRECTORY ${RESOURCES}/jdk-lite/bin/server)

#    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIRECTORY})
#    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_DIRECTORY})
#    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_DIRECTORY})
#else()
#    add_definitions(-DDYNAMIC_JDK_LINK)
#endif()

include_directories(src)
include_directories(${wxWidgets_INCLUDE_DIRS})
#include_directories($ENV{JDK}/include)
#include_directories($ENV{JDK}/include/win32)
include_directories($ENV{OPENSSL}/include)

add_link_options(-static)
if(CMAKE_BUILD_TYPE MATCHES "Release")
    add_link_options(-s)
endif()

link_directories(${wxWidgets_LIB_DIR})
link_directories($ENV{OPENSSL}/lib/gcc_libx64)

#if(STATIC_JDK_LINK MATCHES ON)
#    link_directories($ENV{JDK}/lib)
#endif()

add_executable(${PROJECT_NAME}

        #src/rc/res.rc
        src/main.cpp
        src/ui/mainframe/mainframe.cpp
        src/panel/panel.cpp
        src/panel/basepanel.cpp
        src/elements/textctrl.cpp
        src/ctrl/secure.cpp
        src/ctrl/dataexchange.cpp
        src/ctrl/maindata.cpp
        src/tools/file.cpp
)

set_target_properties(${PROJECT_NAME}
        PROPERTIES
        OUTPUT_NAME ${PROJECT_NAME}-${CMAKE_BUILD_TYPE}-${PROJECT_VERSION}
)

target_link_libraries(${PROJECT_NAME} ${wxWidgets_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ssl)
target_link_libraries(${PROJECT_NAME} crypto)
target_link_libraries(${PROJECT_NAME} crypt32)
target_link_libraries(${PROJECT_NAME} ws2_32)

#if(STATIC_JDK_LINK MATCHES ON)
#    target_link_libraries(${OUTPUT_NAME} jvm)
#endif()