cmake_minimum_required(VERSION 4.2)
project(HotCoffeeEngine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Subdirectories
# Assimp
#add_subdirectory(dependencies/assimp-6.0.4)

# Sfml 
#set(BUILD_SHARED_LIBS ON)
#add_subdirectory(dependencies/SFML-3.0.2)

# Hot Coffee
#add_subdirectory(hcAssetLoaders)
add_subdirectory(hcCore)
add_subdirectory(hcEditor)
#add_subdirectory(hcGraphicsOpenGL)
add_subdirectory(hcUtilities)
#add_subdirectory(hcWindowSfml)

# Editor as startup project
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT hcEditor)
