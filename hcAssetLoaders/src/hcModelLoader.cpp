#include "hc/hcModelLoader.h"

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
