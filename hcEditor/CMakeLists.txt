# Editor Application
file(GLOB_RECURSE EDITOR_HEADERS "include/*.h")
file(GLOB_RECURSE EDITOR_SOURCES "src/*.cpp")

# Grouping source files in IDEs
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/include" 
             PREFIX "Header Files" 
             FILES ${EDITOR_HEADERS})

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/src" 
             PREFIX "Source Files" 
             FILES ${EDITOR_SOURCES})

# Create the executable target
add_executable(hcEditor 
  ${EDITOR_SOURCES} 
  ${EDITOR_HEADERS}
)

# Target properties
set_target_properties(hcEditor PROPERTIES
  OUTPUT_NAME "hcEditor"
  DEBUG_POSTFIX "-d"
  VS_CHARSET "" # Character set: Not set (default)
)

# Compiler options
target_compile_options(hcEditor PRIVATE
  # C++ Warning level
  $<$<CXX_COMPILER_ID:MSVC>:/W4>
  $<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra>
  $<$<CXX_COMPILER_ID:Clang>:-Wall -Wextra>
  
  # Multiprocessor compilation
  $<$<CXX_COMPILER_ID:MSVC>:/MP>

  # SDL checks
  $<$<CXX_COMPILER_ID:MSVC>:/sdl>
)

# Required dependencies (loaded at runtime)
add_dependencies(hcEditor 
  hcGraphicsOpenGL 
  hcAssetLoaders
  hcWindowSfml
)

# Includes
target_include_directories(hcEditor 
  PUBLIC "include"
  PUBLIC "${CMAKE_SOURCE_DIR}/hcUtilities/include"
  PUBLIC "${CMAKE_SOURCE_DIR}/hcCore/include"
)

# Linking
target_link_libraries(hcEditor PUBLIC hcCore hcUtilities imgui)
