#include "hc/hcModelLoader.h"
#include <assimp/Importer.hpp>

namespace hc
{
  ModelLoader::ModelLoader()
    : IModelLoader()
  {
  }

  ModelLoader::~ModelLoader()
  {
  }

  SharedPtr<Model> ModelLoader::load(const Path& filePath)
  {
    // Placeholder implementation
    // Actual model loading logic should be implemented here
    return nullptr;
  }
}
