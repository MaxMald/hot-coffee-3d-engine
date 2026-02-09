#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  class Model;
  class IMesh;

  /**
   * @brief Interface for mesh factory classes.
   *
   * Provides an abstract interface for creating mesh instances from models.
   * Implementations should provide concrete mesh creation logic.
   */
  class IMeshFactory
  {
  public:
    virtual ~IMeshFactory() = default;

    /**
     * @brief Creates a mesh from the given model.
     *
     * @param model Shared pointer to the model used for mesh creation.
     *
     * @return Shared pointer to the created mesh instance. nullptr if creation
     * fails.
     */
    virtual SharedPtr<IMesh> createMesh(SharedPtr<Model> model) = 0;

  protected:
    IMeshFactory() = default;
  };
}
