#pragma once

#include "hc/hcGraphicsOpenGlPrerequisites.h"

namespace hc
{
  class OpenGlMeshManager : public IMeshManager
  {
  public:
    OpenGlMeshManager();
    ~OpenGlMeshManager() override;

    SharedPtr<IMesh> createMeshFromPath(const Path& path) override;
    SharedPtr<IMesh> createMeshFromModel(const SharedPtr<Model>& model) override;

  private:
  };
}
