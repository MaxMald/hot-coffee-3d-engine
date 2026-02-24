# Window Sfml library
file(GLOB_RECURSE WINDOW_SFML_HEADERS "include/*.h")
file(GLOB_RECURSE WINDOW_SFML_SOURCES "src/*.cpp")

# Grouping source files in IDEs
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/include" 
             PREFIX "Header Files" 
             FILES ${WINDOW_SFML_HEADERS})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/src"
             PREFIX "Source Files"
             FILES ${WINDOW_SFML_SOURCES})

# Create the shared library target
add_library(hcWindowSfml SHARED 
  ${WINDOW_SFML_SOURCES} 
  ${WINDOW_SFML_HEADERS}
)

# Target properties
set_target_properties(hcWindowSfml PROPERTIES
  OUTPUT_NAME "hcWindowSfml"
  DEBUG_POSTFIX "-d"
  VS_CHARSET "" # Character set: Not set (default)
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

##Post-build commands
# copy sfml libraries to the output directory of the Hot Coffee Engine
if(WIN32)
  add_custom_command(TARGET hcWindowSfml POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      "${CMAKE_BINARY_DIR}/dependencies/SFML-3.0.2/bin/$<CONFIG>/sfml-system$<$<CONFIG:Debug>:-d>-3.dll"
      "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIG>/"
      "${CMAKE_BINARY_DIR}/dependencies/SFML-3.0.2/bin/$<CONFIG>/sfml-window$<$<CONFIG:Debug>:-d>-3.dll"
      "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIG>/"
  )
endif()

# Install the library to the 'lib' directory
install(TARGETS hcWindowSfml DESTINATION lib)
