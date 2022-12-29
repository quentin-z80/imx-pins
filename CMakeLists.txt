cmake_minimum_required(VERSION 3.2.3)

project(
  imx-pins
  VERSION 1.0
  LANGUAGES CXX)

### Require out-of-source builds
file(TO_CMAKE_PATH "${PROJECT_BINARY_DIR}/CMakeLists.txt" LOC_PATH)
if(EXISTS "${LOC_PATH}")
    message(FATAL_ERROR "You cannot build in a source directory (or any directory with a CMakeLists.txt file). Please make a build subdirectory. Feel free to remove CMakeCache.txt and CMakeFiles.")
endif()

add_subdirectory(src)

add_custom_target(copy_pinsettings ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_SOURCE_DIR}/pinsettings
        ${PROJECT_BINARY_DIR}/share/imx-pins/pinsettings
        COMMENT "Copying pinsettings into binary directory")

add_dependencies(${PROJECT_NAME} copy_pinsettings)
