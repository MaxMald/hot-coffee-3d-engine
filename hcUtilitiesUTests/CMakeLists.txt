# Utilities Unit Tests Application

include(FetchContent)

# 1. Download and unpack GoogleTest automatically
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.zip
)
# Prevent GTest from overriding global compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Utilities Unit Tests Application
file(GLOB_RECURSE UTILITIES_UTESTS_HEADERS "include/*.h")
file(GLOB_RECURSE UTILITIES_UTESTS_SOURCES "src/*.cpp")

# Grouping source files in IDEs
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/include" 
             PREFIX "Header Files" 
             FILES ${UTILITIES_UTESTS_HEADERS})

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/src" 
             PREFIX "Source Files" 
             FILES ${UTILITIES_UTESTS_SOURCES})

# Define the unit test executable
add_executable(hcUtilitiesUTests
${UTILITIES_UTESTS_SOURCES} 
${UTILITIES_UTESTS_HEADERS}
)

# Target properties
set_target_properties(hcUtilitiesUTests PROPERTIES
  OUTPUT_NAME "hcUtilitiesUTests"
  DEBUG_POSTFIX "-d"
  VS_CHARSET "" # Character set: Not set (default)
)

target_include_directories(hcUtilitiesUTests PRIVATE
  "include"
  "${CMAKE_SOURCE_DIR}/hcUtilities/include"
)

# Link against your app logic library and GoogleTest
# gtest_main automatically provides a standard main() function for running tests
target_link_libraries(hcUtilitiesUTests PRIVATE
hcUtilities
GTest::gtest_main
)

# 4. Enable GoogleTest discovery so individual test cases show up in CTest
include(GoogleTest)
gtest_discover_tests(hcUtilitiesUTests)
