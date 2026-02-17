cmake_minimum_required(VERSION 4.2)
project(HotCoffeeEngine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Current shared libs state
set(_OLD_BUILD_SHARED_LIBS ${BUILD_SHARED_LIBS})

## Subdirectories
# Assimp
set(BUILD_SHARED_LIBS ON)
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ZLIB ON CACHE BOOL "" FORCE)

add_subdirectory(dependencies/assimp-6.0.4)

set_target_properties(
  assimp
  zlibstatic
  uninstall
  UpdateAssimpLibsDebugSymbolsAndDLLs
  PROPERTIES FOLDER "Assimp"
)

set(BUILD_SHARED_LIBS ${_OLD_BUILD_SHARED_LIBS})

# Sfml 
set(BUILD_SHARED_LIBS ON)
set(SFML_BUILD_NETWORK OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_AUDIO OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_GRAPHICS OFF CACHE BOOL "" FORCE)
add_subdirectory(dependencies/SFML-3.0.2)
set(BUILD_SHARED_LIBS ${_OLD_BUILD_SHARED_LIBS})

# Hot Coffee
add_subdirectory(hcAssetLoaders)
add_subdirectory(hcCore)
add_subdirectory(hcEditor)
add_subdirectory(hcGraphicsOpenGL)
add_subdirectory(hcUtilities)
add_subdirectory(hcWindowSfml)

# Editor as startup project
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT hcEditor)
