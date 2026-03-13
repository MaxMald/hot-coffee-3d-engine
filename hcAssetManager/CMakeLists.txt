# Asset Manager library
file(GLOB_RECURSE ASSET_MANAGER_HEADERS "include/*.h")
file(GLOB_RECURSE ASSET_MANAGER_SOURCES "src/*.cpp")
             
# Grouping source files in IDEs
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/include" 
             PREFIX "Header Files" 
             FILES ${ASSET_MANAGER_HEADERS})

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/src" 
             PREFIX "Source Files" 
             FILES ${ASSET_MANAGER_SOURCES})

# Create the shared library target
add_library(hcAssetManager SHARED 
  ${ASSET_MANAGER_SOURCES} 
  ${ASSET_MANAGER_HEADERS}
)

# Target properties
set_target_properties(hcAssetManager PROPERTIES
  OUTPUT_NAME "hcAssetManager"
  DEBUG_POSTFIX "-d"
  VS_CHARSET "" # Character set: Not set (default)
)

# Compiler options
target_compile_options(hcAssetManager PRIVATE
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
target_include_directories(hcAssetManager 
  PUBLIC "include"
  PRIVATE "${CMAKE_SOURCE_DIR}/dependencies/assimp-6.0.4/include"
  PRIVATE "${CMAKE_SOURCE_DIR}/dependencies/NOTHINGS_STB/include"
)

# Linking
target_link_libraries(hcAssetManager PUBLIC hcCore hcUtilities assimp)

# Install the library to the 'lib' directory
install(TARGETS hcAssetManager DESTINATION lib)
