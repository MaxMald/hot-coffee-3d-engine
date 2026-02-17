# Window Sfml library
file(GLOB_RECURSE WINDOW_SFML_HEADERS "include/*.h")
file(GLOB_RECURSE WINDOW_SFML_SOURCES "src/*.cpp")

add_library(hcWindowSfml SHARED 
  ${WINDOW_SFML_SOURCES} 
  ${WINDOW_SFML_HEADERS}
)

# Target properties
set_target_properties(hcWindowSfml PROPERTIES
  OUTPUT_NAME "hcWindowSfml"
  SUFFIX "-d"
)

# Compiler options
target_compile_options(hcWindowSfml PRIVATE
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
target_include_directories(hcWindowSfml 
  PUBLIC "include"
  PUBLIC "${CMAKE_SOURCE_DIR}/hcUtilities/include"
  PUBLIC "${CMAKE_SOURCE_DIR}/hcCore/include"
  PRIVATE "${CMAKE_SOURCE_DIR}/dependencies/SFML-3.0.2/include"
)

# Linking
target_link_libraries(hcWindowSfml PUBLIC 
  hcUtilities 
  hcCore 
  sfml-window
  sfml-system
)

# Install the library to the 'lib' directory
install(TARGETS hcWindowSfml DESTINATION lib)
