# ImGui Library
file(GLOB IMGUI_HEADERS "include/*.h")
file(GLOB IMGUI_SOURCES "src/*.cpp")

if(NOT WIN32)
  list(FILTER IMGUI_SOURCES EXCLUDE REGEX ".*/imgui_impl_win32\\.cpp$")
endif()

# Create a static library for ImGui
add_library(imgui STATIC
  ${IMGUI_SOURCES}
  ${IMGUI_HEADERS}
)

# Preprocessor definition
target_compile_definitions(imgui PUBLIC USE_STD_FILESYSTEM)

# Public include directory
target_include_directories(imgui PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

# Set target properties
set_target_properties(imgui PROPERTIES
  OUTPUT_NAME "imgui"
  DEBUG_POSTFIX "-d"
  FOLDER "Imgui"
)
