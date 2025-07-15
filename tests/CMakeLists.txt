# Contributor: Sarah
# Purpose: configure GTests to compile and run

cmake_minimum_required(VERSION 3.10)
project(PradoEditor)

set(CMAKE_CXX_STANDARD 17)

enable_testing()

# find packages used
find_package(OpenCV REQUIRED)
find_package(GTest REQUIRED)

include_directories(
    ${OpenCV_INCLUDE_DIRS}
    ${GTEST_INCLUDE_DIRS}
    include          
    src              
)

add_executable(OpenCVTest
    src/artwork.cpp
    src/EditorManager.cpp
    src/prado_editor.cpp
)
target_link_libraries(OpenCVTest ${OpenCV_LIBS})

add_executable(runTests
    tests/testing.cpp  
    src/EditorManager.cpp
    src/ImageCache.cpp
    src/artwork.cpp                 
)

target_link_libraries(runTests
    ${OpenCV_LIBS}
    GTest::GTest
    GTest::Main
    pthread
)

add_test(NAME PradoEditorTests COMMAND runTests)
