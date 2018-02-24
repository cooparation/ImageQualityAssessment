cmake_minimum_required(VERSION 2.8.0)
project(ImageQualityAssessment)

option(SDK_TEST "whether have a test" ON)

include_directories(./include)

find_package(OpenCV)
if (OpenCV_FOUND)
    include_directories(${OpenCV_INCLUDE_DIRS})
    message(STATUS "OpenCV VERSION" ${OpenCV_VERSION})
else()
    message(FATAL_ERROR "opencv not found")
endif()

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++11 -fPIC")
set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-fPIC")

set(SRC_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ImageQualityAssessment.cpp
    )

add_library(iqa SHARED ${SRC_FILES})

target_link_libraries(iqa ${OpenCV_LIBS})
if (SDK_TEST)
    set(TEST_FILES ./tests/main.cpp)
    add_executable(test ${TEST_FILES})
    target_link_libraries(test
        iqa
        ${OpenCV_LIBS})
endif()
