# Asset Loaders library
file(GLOB ASSET_LOADERS_SOURCES "src/*.cpp")
add_library(hcAssetLoaders ${ASSET_LOADERS_SOURCES})

# Dependencies
target_include_directories(hcAssetLoaders 
  PUBLIC "include"
  PRIVATE "${CMAKE_SOURCE_DIR}/dependencies/assimp-6.0.4/include"
  PRIVATE "${CMAKE_SOURCE_DIR}/dependencies/NOTHINGS_STB/include"
)

# Linking
target_link_libraries(hcAssetLoaders PUBLIC hcCore hcUtilities assimp)
