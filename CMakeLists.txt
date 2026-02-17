cmake_minimum_required(VERSION 3.20)
project(HotCoffeeEngine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Subdirectories
add_subdirectory(dependencies/assimp-6.0.4)
add_subdirectory(hcAssetLoaders)
add_subdirectory(hcCore)
add_subdirectory(hcEditor)
add_subdirectory(hcGraphicsOpenGL)
add_subdirectory(hcUtilities)
add_subdirectory(hcWindowSfml)
