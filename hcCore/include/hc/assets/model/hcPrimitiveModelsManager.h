#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/assets/model/hcPrimitiveModelType.h"

namespace hc
{
  class Model;

  /**
   * Manages and caches primitive 3D model assets for efficient reuse.
   * 
   * This manager provides lazy loading and caching of basic geometric
   * primitives such as cubes, spheres, cylinders, cones, planes, and
   * pyramids. Models are created on-demand and stored for subsequent
   * requests.
   */
  class PrimitiveModelsManager : public NonCopyable
  {
  public:
    PrimitiveModelsManager() = default;
    ~PrimitiveModelsManager() = default;

    /**
     * Retrieves a primitive model, creating it if necessary.
     * 
     * If the requested primitive has not been created yet, it will be
     * generated and cached for future use.
     * 
     * @param primitiveType The type of primitive model to retrieve.
     * 
     * @return A shared pointer to the requested primitive model.
     */
    SharedPtr<Model> getPrimitive(primitiveModelType::Type primitiveType);

    /**
     * Destroys all cached primitive models and releases their resources.
     * 
     * Clears the internal cache, freeing memory associated with all
     * stored primitive models.
     */
    void destroy();

  private:
    UnorderedMap<primitiveModelType::Type, SharedPtr<Model>> m_primitiveModels;

    bool hasPrimitive(primitiveModelType::Type primitiveType) const;
  };
}
