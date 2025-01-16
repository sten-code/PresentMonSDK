cmake_minimum_required(VERSION 3.20)
project(PresentMonSDK)
include(FetchContent)
set(CMAKE_CXX_STANDARD 23)

# The packages that are required by PresentMon
find_package(boost_asio CONFIG REQUIRED)
find_package(boost_process CONFIG REQUIRED)
find_package(boost_interprocess CONFIG REQUIRED)
find_package(boost_process CONFIG REQUIRED)
find_package(cereal CONFIG REQUIRED)
find_package(unofficial-concurrentqueue CONFIG REQUIRED)
find_package(GTest CONFIG REQUIRED)
find_package(CLI11 CONFIG REQUIRED)
find_path(WINREG_INCLUDE_DIRS "winreg/WinReg.hpp")

# PresentMon sources
file(GLOB_RECURSE PRESENT_MON_SOURCES IntelPresentMon/**/*.cpp)
add_library(${PROJECT_NAME} ${PRESENT_MON_SOURCES})
add_definitions(-DPRESENTMONAPI2_EXPORTS)
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DIS_DEBUG=1)
else()
    add_definitions(-DIS_DEBUG=0)
endif()

# Generate EnumMetric.h
add_custom_command(
        OUTPUT ${PROJECT_SOURCE_DIR}/IntelPresentMon/Interprocess/source/metadata/EnumMetric.h
        COMMAND ${CMAKE_COMMAND} -E env "PATH=$ENV{PATH}" ${PROJECT_SOURCE_DIR}/generate/awk.exe -f ${PROJECT_SOURCE_DIR}/generate/metrics.awk ${PROJECT_SOURCE_DIR}/generate/metrics.csv > ${PROJECT_SOURCE_DIR}/IntelPresentMon/Interprocess/source/metadata/EnumMetric.h
        DEPENDS ${PROJECT_SOURCE_DIR}/generate/metrics.awk ${PROJECT_SOURCE_DIR}/generate/metrics.csv
        COMMENT "Generating EnumMetric.h"
)
add_custom_target(generate_enum_metric DEPENDS ${PROJECT_SOURCE_DIR}/IntelPresentMon/Interprocess/source/metadata/EnumMetric.h)

# Generate build_id.h
add_custom_command(
        OUTPUT ${PROJECT_SOURCE_DIR}/IntelPresentMon/Versioning/generated/build_id.h
        COMMAND ${CMAKE_COMMAND} -E env "PATH=$ENV{PATH}" cmd /c ${PROJECT_SOURCE_DIR}/IntelPresentMon/Versioning/scripts/pre-build.bat
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/IntelPresentMon/Versioning
        DEPENDS ${PROJECT_SOURCE_DIR}/IntelPresentMon/Versioning/scripts/pre-build.bat
        COMMENT "Running pre-build script to generate build_id.h"
)
add_custom_target(generate_build_id DEPENDS ${PROJECT_SOURCE_DIR}/IntelPresentMon/Versioning/generated/build_id.h)
add_dependencies(${PROJECT_NAME} generate_enum_metric generate_build_id)

target_link_libraries(${PROJECT_NAME} PRIVATE
        boost_asio
        boost_process
        boost_interprocess
        CLI11::CLI11
        cereal::cereal
        unofficial::concurrentqueue::concurrentqueue
        GTest::gtest
        Shlwapi
)

target_include_directories(${PROJECT_NAME} PRIVATE
        IntelPresentMon/Interprocess/source # Required for metadata/EnumMetric.h
        ${WINREG_INCLUDE_DIRS}
        $ENV{WindowsSdkDir}/Include/um
)
target_include_directories(${PROJECT_NAME} PUBLIC IntelPresentMon)
