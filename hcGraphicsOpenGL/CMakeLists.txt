#includes
include(GNUInstallDirs)
include(FetchContent)

# Prerequisites
find_package(OpenGL REQUIRED)

if(UNIX)
  find_package(GLEW REQUIRED)
endif()

# Predict SPIRV-Tools source location (FetchContent default pattern)
set(_SPIRV_TOOLS_SRC "${CMAKE_BINARY_DIR}/_deps/SPIRV-Tools-src")

# Declare SPIRV-Headers to download into spirv-tools/external/spirv-headers
FetchContent_Declare(
  SPIRV-Headers
  GIT_REPOSITORY https://github.com/KhronosGroup/SPIRV-Headers.git
  GIT_TAG vulkan-sdk-1.3.280
  SOURCE_DIR "${_SPIRV_TOOLS_SRC}/external/spirv-headers"
)

# Declare SPIRV-Tools
FetchContent_Declare(
  SPIRV-Tools
  GIT_REPOSITORY https://github.com/KhronosGroup/SPIRV-Tools.git
  GIT_TAG v2024.1
)
set(SKIP_SPIRV_TOOLS_TESTS ON CACHE BOOL "" FORCE)
set(SPIRV_SKIP_EXECUTABLES ON CACHE BOOL "" FORCE)

# Fetch glslang (required by shaderc)
FetchContent_Declare(
  glslang
  GIT_REPOSITORY https://github.com/KhronosGroup/glslang.git
  GIT_TAG 14.1.0
)
set(SKIP_GLSLANG_TESTS ON CACHE BOOL "" FORCE)
set(ENABLE_HLSL ON CACHE BOOL "" FORCE)

# Fetch shaderc
FetchContent_Declare(
  shaderc
  GIT_REPOSITORY https://github.com/google/shaderc.git
  GIT_TAG v2024.1
)
set(SHADERC_SKIP_TESTS ON CACHE BOOL "" FORCE)
set(SHADERC_SKIP_EXAMPLES ON CACHE BOOL "" FORCE)

# Make SPIRV-Headers available FIRST (downloads headers)
FetchContent_MakeAvailable(SPIRV-Headers)

# Then make everything else available (SPIRV-Headers will be in place)
FetchContent_MakeAvailable(SPIRV-Tools glslang shaderc)

# Group dependencies into a folder
set_target_properties(glslang PROPERTIES FOLDER "Dependencies")
set_target_properties(glslc_exe PROPERTIES FOLDER "Dependencies")
set_target_properties(glslc PROPERTIES FOLDER "Dependencies")
set_target_properties(extinst_tables PROPERTIES FOLDER "Dependencies")
set_target_properties(enum_string_mapping PROPERTIES FOLDER "Dependencies")
set_target_properties(core_tables PROPERTIES FOLDER "Dependencies")
set_target_properties(check-copyright PROPERTIES FOLDER "Dependencies")
set_target_properties(build-version PROPERTIES FOLDER "Dependencies")
set_target_properties(add-copyright PROPERTIES FOLDER "Dependencies")

set_target_properties(shaderc PROPERTIES FOLDER "Dependencies")
set_target_properties(shaderc_combined PROPERTIES FOLDER "Dependencies")
set_target_properties(shaderc_combined-pkg-config PROPERTIES FOLDER "Dependencies")
set_target_properties(shaderc_shared PROPERTIES FOLDER "Dependencies")
set_target_properties(shaderc_static-pkg-config PROPERTIES FOLDER "Dependencies")
set_target_properties(shaderc_util PROPERTIES FOLDER "Dependencies")
set_target_properties(shaderc-pkg-config PROPERTIES FOLDER "Dependencies")
set_target_properties(spirv-tools-pkg-config PROPERTIES FOLDER "Dependencies")
set_target_properties(spirv-tools-shared-pkg-config PROPERTIES FOLDER "Dependencies")
set_target_properties(testdata PROPERTIES FOLDER "Dependencies")


# Graphics OpenGl library
file(GLOB_RECURSE GRAPHICS_OPENGL_HEADERS "include/*.h")
file(GLOB_RECURSE GRAPHICS_OPENGL_SOURCES "src/*.cpp")

# Grouping source files in IDEs
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/include" 
             PREFIX "Header Files" 
             FILES ${GRAPHICS_OPENGL_HEADERS})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/src"
             PREFIX "Source Files" 
             FILES ${GRAPHICS_OPENGL_SOURCES})

# Create the shared library target
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
# target_link_libraries(hcGraphicsOpenGL PRIVATE shaderc)

if(WIN32)
  target_link_libraries(hcGraphicsOpenGL PRIVATE OpenGL::GL)
  target_link_libraries(hcGraphicsOpenGL PRIVATE "${CMAKE_SOURCE_DIR}/dependencies/GLEW_2.3.0/lib/Windows/Release/${HC_ARCH}/glew32.lib")
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

# copy the whole shaders/opengl folder next to the built target
add_custom_command(TARGET hcGraphicsOpenGL POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/shaders/opengl"
    "$<TARGET_FILE_DIR:hcGraphicsOpenGL>/shaders/opengl"
)

# Install
install(TARGETS hcGraphicsOpenGL DESTINATION lib)
install(DIRECTORY "${CMAKE_SOURCE_DIR}/shaders/opengl"
        DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/shaders/opengl"
        FILE_PERMISSIONS OWNER_READ GROUP_READ WORLD_READ)
