#pragma once

#include "hc/hcAssetManagerPrerequisites.h"

namespace hc
{
  class ModelAssetManager : public IModelAssetManager
  {
  public:
    ModelAssetManager();
    virtual ~ModelAssetManager();

    SharedPtr<Model> load(const Path& path) override;
    SharedPtr<Model> get(const Path& path) const override;
    bool isLoaded(const Path& path) const override;
    void clear() override;
    SharedPtr<Model> getPrimitive(primitiveModelType::Type primitiveType) override;
  };
}
