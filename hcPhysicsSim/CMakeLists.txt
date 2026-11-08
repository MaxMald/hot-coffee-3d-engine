# Editor Application
file(GLOB_RECURSE PHYSICS_SIM_HEADERS "include/*.h")
file(GLOB_RECURSE PHYSICS_SIM_SOURCES "src/*.cpp")

# Grouping source files in IDEs
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/include" 
             PREFIX "Header Files" 
             FILES ${PHYSICS_SIM_HEADERS})

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/src" 
             PREFIX "Source Files" 
             FILES ${PHYSICS_SIM_SOURCES})

# Create the executable target
add_executable(hcPhysicsSim
  ${PHYSICS_SIM_SOURCES} 
  ${PHYSICS_SIM_HEADERS}
)

# Target properties
set_target_properties(hcPhysicsSim PROPERTIES
  OUTPUT_NAME "hcPhysicsSim"
  DEBUG_POSTFIX "-d"
  VS_CHARSET "" # Character set: Not set (default)
  VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:hcPhysicsSim>" # Set Visual Studio debugger working directory to the target output dir for hcPhysicsSim
)

# Compiler options
target_compile_options(hcPhysicsSim PRIVATE
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
add_dependencies(hcPhysicsSim 
  hcGraphicsOpenGL 
  hcAssetManager
  hcWindowSfml
)

# Includes
target_include_directories(hcPhysicsSim 
  PUBLIC "include"
  PUBLIC "${CMAKE_SOURCE_DIR}/hcUtilities/include"
  PUBLIC "${CMAKE_SOURCE_DIR}/hcCore/include"
)

# Linking
target_link_libraries(hcPhysicsSim PUBLIC hcCore hcUtilities)
