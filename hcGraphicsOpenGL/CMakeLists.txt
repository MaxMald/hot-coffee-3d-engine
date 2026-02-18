# Prerequisites
find_package(OpenGL REQUIRED)

if(UNIX)
  find_package(GLEW REQUIRED)
endif()

# Graphics OpenGl library
file(GLOB_RECURSE GRAPHICS_OPENGL_HEADERS "include/*.h")
file(GLOB_RECURSE GRAPHICS_OPENGL_SOURCES "src/*.cpp")

add_library(hcGraphicsOpenGL SHARED 
  ${GRAPHICS_OPENGL_SOURCES} 
  ${GRAPHICS_OPENGL_HEADERS}
)

# Target properties
set_target_properties(hcGraphicsOpenGL PROPERTIES
  OUTPUT_NAME "hcGraphicsOpenGL"
  DEBUG_POSTFIX "-d"
  VS_CHARSET "" # Character set: Not set (default)
)

# Compiler options
target_compile_options(hcGraphicsOpenGL PRIVATE
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
target_include_directories(hcGraphicsOpenGL 
  PUBLIC "include"
  PUBLIC "${CMAKE_SOURCE_DIR}/hcUtilities/include"
  PUBLIC "${CMAKE_SOURCE_DIR}/hcCore/include"
  PRIVATE "${CMAKE_SOURCE_DIR}/dependencies/GLEW_2.3.0/include"
)

# Linking
target_link_libraries(hcGraphicsOpenGL PUBLIC hcCore hcUtilities)

if(WIN32)
  target_link_libraries(hcGraphicsOpenGL PRIVATE OpenGL::GL)
  target_link_libraries(hcGraphicsOpenGL PRIVATE "${CMAKE_SOURCE_DIR}/dependencies/GLEW_2.3.0/lib/Windows/Release/x64/glew32.lib")
elseif(UNIX)
  target_link_libraries(hcGraphicsOpenGL PRIVATE OpenGL::GL GLEW::GLEW)
endif()

##Post-build commands
# copy glew to the output directory of the Hot Coffee Engine
if(WIN32)
  add_custom_command(TARGET hcGraphicsOpenGL POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      "${CMAKE_SOURCE_DIR}/dependencies/GLEW_2.3.0/bin/Windows/Release/${HC_ARCH}/glew32.dll"
      "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIG>/"
  )
endif()

# Install
install(TARGETS hcGraphicsOpenGL DESTINATION lib)
