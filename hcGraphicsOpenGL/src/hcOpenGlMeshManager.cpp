#include "hc/hcOpenGlMeshManager.h"

namespace hc
{
  OpenGlMeshManager::OpenGlMeshManager()
  {
  }

  OpenGlMeshManager::~OpenGlMeshManager()
  {
  }

  SharedPtr<IMesh> OpenGlMeshManager::createMeshFromPath(const Path& path)
  {
    return SharedPtr<IMesh>();
  }

  SharedPtr<IMesh> OpenGlMeshManager::createMeshFromModel(const SharedPtr<Model>& model)
  {
    return SharedPtr<IMesh>();
  }
}
