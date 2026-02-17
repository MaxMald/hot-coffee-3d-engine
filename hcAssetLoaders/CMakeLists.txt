# Asset Loaders library
file(GLOB_RECURSE ASSET_LOADERS_HEADERS "include/*.h")
file(GLOB_RECURSE ASSET_LOADERS_SOURCES "src/*.cpp")

add_library(hcAssetLoaders SHARED 
  ${ASSET_LOADERS_SOURCES} 
  ${ASSET_LOADERS_HEADERS}
)

# Target properties
set_target_properties(hcAssetLoaders PROPERTIES
  OUTPUT_NAME "hcAssetLoaders"
  SUFFIX "-d"
)

# Compiler options
target_compile_options(hcAssetLoaders PRIVATE
  # C++ Warning level
  $<$<CXX_COMPILER_ID:MSVC>:/W4>
  $<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra>
  $<$<CXX_COMPILER_ID:Clang>:-Wall -Wextra>
  
  # Multiprocessor compilation
  $<$<CXX_COMPILER_ID:MSVC>:/MP>

  # SDL checks
  $<$<CXX_COMPILER_ID:MSVC>:/sdl>
)

# Includes
target_include_directories(hcAssetLoaders 
  PUBLIC "include"
  PRIVATE "${CMAKE_SOURCE_DIR}/dependencies/assimp-6.0.4/include"
  PRIVATE "${CMAKE_SOURCE_DIR}/dependencies/NOTHINGS_STB/include"
)

# Linking
target_link_libraries(hcAssetLoaders PUBLIC hcCore hcUtilities assimp)

# Install the library to the 'lib' directory
install(TARGETS hcAssetLoaders DESTINATION lib)
