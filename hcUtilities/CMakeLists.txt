# Utilities library
file(GLOB_RECURSE UTILITIES_HEADERS "include/*.h")
file(GLOB_RECURSE UTILITIES_SOURCES "src/*.cpp")

# Grouping source files in IDEs
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/include" 
             PREFIX "Header Files" 
             FILES ${UTILITIES_HEADERS})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/src"
             PREFIX "Source Files"
             FILES ${UTILITIES_SOURCES})

# Create the shared library target
add_library(hcUtilities SHARED 
  ${UTILITIES_SOURCES} 
  ${UTILITIES_HEADERS}
)

# Target properties
set_target_properties(hcUtilities PROPERTIES
  OUTPUT_NAME "hcUtilities"
  DEBUG_POSTFIX "-d"
  VS_CHARSET "" # Character set: Not set (default)
)

# Compiler options
target_compile_options(hcUtilities PRIVATE
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
target_include_directories(hcUtilities 
  PUBLIC "include"
  PRIVATE "${CMAKE_SOURCE_DIR}/dependencies/NLOHMANN_JSON/include"
  PRIVATE "${CMAKE_SOURCE_DIR}/dependencies/STDUUID/include"
)

target_link_libraries(hcUtilities PRIVATE
  $<$<PLATFORM_ID:Linux>:uuid>
)

# Install the library to the 'lib' directory
install(TARGETS hcUtilities DESTINATION lib)
