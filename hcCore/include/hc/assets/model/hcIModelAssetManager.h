#pragma once

#include "hc/assets/hcATypedAssetManager.h"
#include "hc/assets/model/hcModel.h"
#include "hc/assets/model/hcPrimitiveModelType.h"

namespace hc
{
  /**
   * Interface for managing 3D model assets.
   *
   * Extends the typed asset manager to provide specialized functionality
   * for loading and managing 3D models, including built-in primitive
   * models.
   */
  class HC_CORE_EXPORT IModelAssetManager : public ATypedAssetManager<Model>
  {
  public:
    virtual ~IModelAssetManager() = default;

    /**
     * Retrieves a primitive model.
     *
     * @param primitiveType The type of primitive model to retrieve
     *
     * @return A shared pointer to the primitive model
     */
    virtual SharedPtr<Model> getPrimitive(primitiveModelType::Type primitiveType) = 0;

  protected:
    IModelAssetManager() = default;
  };
}
