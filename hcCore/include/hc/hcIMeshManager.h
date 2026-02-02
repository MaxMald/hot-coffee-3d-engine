#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  class IMesh;
  class Model;

  class IMeshManager : public NonCopyable
  {
    virtual ~IMeshManager() = default;

    virtual SharedPtr<IMesh> createMeshFromPath(const Path& path) = 0;
    virtual SharedPtr<IMesh> createMeshFromModel(const SharedPtr<Model>& model) = 0;

  protected:
    IMeshManager() = default;
  };
}
