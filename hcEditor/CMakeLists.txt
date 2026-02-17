# Editor Application
file(GLOB_RECURSE EDITOR_HEADERS "include/*.h")
file(GLOB_RECURSE EDITOR_SOURCES "src/*.cpp")

add_executable(hcEditor 
  ${EDITOR_SOURCES} 
  ${EDITOR_HEADERS}
)

# Target properties
set_target_properties(hcEditor PROPERTIES
  OUTPUT_NAME "hcEditor"
  SUFFIX "-d"
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

# Includes
target_include_directories(hcEditor 
  PUBLIC "include"
  PUBLIC "${CMAKE_SOURCE_DIR}/hcUtilities/include"
  PUBLIC "${CMAKE_SOURCE_DIR}/hcCore/include"
)

# Linking
target_link_libraries(hcEditor PUBLIC hcCore hcUtilities)
