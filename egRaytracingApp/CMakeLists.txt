# Application
file(GLOB_RECURSE RAYT_APPLICATION_HEADERS "include/*.h")
file(GLOB_RECURSE RAYT_APPLICATION_SOURCES "src/*.cpp")
file(GLOB_RECURSE RAYT_APPLICATION_RESOURCES "sources/*.rc" "sources/*.h")

# Grouping source files in IDEs
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/include" 
             PREFIX "Header Files" 
             FILES ${RAYT_APPLICATION_HEADERS})

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/src" 
             PREFIX "Source Files" 
             FILES ${RAYT_APPLICATION_SOURCES})

# Force Windows headers to use Unicode character sets
# If the project is not using Unicode, it won't be able to compile (create the window).
add_definitions(-DUNICODE -D_UNICODE)

# Create the executable target
# The WIN32 flag configures the WinMain entry point and hides the console
add_executable(egRaytracingApp WIN32
  ${RAYT_APPLICATION_SOURCES} 
  ${RAYT_APPLICATION_HEADERS}
  ${RAYT_APPLICATION_RESOURCES}
)

# Target properties
set_target_properties(egRaytracingApp PROPERTIES
  OUTPUT_NAME "egRaytracingApp"
  DEBUG_POSTFIX "-d"
)

# Compiler options
target_compile_options(egRaytracingApp PRIVATE
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
add_dependencies(egRaytracingApp
  hcUtilities
  hcCore
  hcAssetManager
)

# Includes
target_include_directories(egRaytracingApp
  PUBLIC "include"
  PUBLIC "${CMAKE_SOURCE_DIR}/hcUtilities/include"
  PUBLIC "${CMAKE_SOURCE_DIR}/hcCore/include"
  PUBLIC "${CMAKE_SOURCE_DIR}/hcAssetManager/include"
)

# Linking
target_link_libraries(egRaytracingApp PUBLIC
	hcUtilities 
	hcCore
	hcAssetManager
)