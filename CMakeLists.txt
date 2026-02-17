cmake_minimum_required(VERSION 3.20)
project(HotCoffeeEngine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(hcAssetLoaders)
add_subdirectory(hcCore)
add_subdirectory(hcEditor)
add_subdirectory(hcGraphicsOpenGL)
add_subdirectory(hcUtilities)
add_subdirectory(hcWindowSfml)
