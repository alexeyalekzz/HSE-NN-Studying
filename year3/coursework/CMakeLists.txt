cmake_minimum_required(VERSION 3.15)
include(FetchContent)
set (PROJECT VisualOdometry)
project (${PROJECT})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package( OpenCV 4.0 REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

FetchContent_Declare(
        readerwriterqueue
        GIT_REPOSITORY    https://github.com/cameron314/readerwriterqueue
        GIT_TAG           master
)
FetchContent_MakeAvailable(readerwriterqueue)

find_package(Threads REQUIRED)
find_package(Eigen3 3.1.0 REQUIRED)
find_package(Pangolin REQUIRED)
find_package(Ceres 2.0 REQUIRED)
find_package(Sophus REQUIRED)


if (MSVC)
    set(G2O_ROOT "C:/Program Files (x86)/g2o")
    find_package(G2O REQUIRED)
    include_directories("${G2O_ROOT}/include")
    file(GLOB G2O_LIBS "${G2O_ROOT}/lib/*.lib")
else ()
    find_package(G2O REQUIRED)
endif ()

include_directories(./include)

file(GLOB MyCSources ./src/*.cpp)
set(SOURCES ${MyCSources})
source_group("Source Files" FILES ${SOURCES})

add_executable( ${PROJECT}  ${HEADERS} ${SOURCES} )
target_link_libraries( ${PROJECT} ${OpenCV_LIBS} Sophus::Sophus
        Eigen3::Eigen readerwriterqueue pango_display ${CERES_LIBRARIES} ${G2O_LIBS})
set_property(TARGET ${PROJECT} PROPERTY
        MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")

set(OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${OUTPUT_DIRECTORY}/Debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${OUTPUT_DIRECTORY}/Release)

if (MSVC)
    add_custom_command(TARGET ${PROJECT} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            "${CMAKE_SOURCE_DIR}/external/GLEW/bin/glew32.dll"
            "$<$<CONFIG:Debug>:${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}>"
            "$<$<CONFIG:Release>:${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}>"
            )
    add_custom_command(TARGET ${PROJECT} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            "${CMAKE_SOURCE_DIR}/external/glog/build/Debug/glogd.dll"
            "$<$<CONFIG:Debug>:${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}>"
            "$<$<CONFIG:Release>:${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}>"
            )
endif ()

# Set working directory
if (MSVC)
    set_property(TARGET ${PROJECT} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${OUTPUT_DIRECTORY}")
elseif (APPLE)
    set_property(TARGET ${PROJECT} PROPERTY XCODE_SCHEME_WORKING_DIRECTORY "${OUTPUT_DIRECTORY}")
endif ()

add_custom_command(TARGET ${PROJECT} PRE_BUILD
        COMMAND if $<CONFIG:Debug> neq 0 (
        "${CMAKE_COMMAND}" -E create_symlink
        "${CMAKE_SOURCE_DIR}/configs"
        "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}/configs"
        )
        COMMAND if $<CONFIG:Release> neq 0 (
        "${CMAKE_COMMAND}" -E create_symlink
        "${CMAKE_SOURCE_DIR}/configs"
        "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}/configs"
        )
)

add_custom_command(TARGET ${PROJECT} PRE_BUILD
        COMMAND if $<CONFIG:Debug> neq 0 (
        "${CMAKE_COMMAND}" -E create_symlink
        "${CMAKE_SOURCE_DIR}/datasets"
        "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}/datasets"
        )
        COMMAND if $<CONFIG:Release> neq 0 (
        "${CMAKE_COMMAND}" -E create_symlink
        "${CMAKE_SOURCE_DIR}/datasets"
        "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}/datasets"
        )
)
