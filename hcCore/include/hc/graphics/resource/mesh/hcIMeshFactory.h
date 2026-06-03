#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  class Model;
  class IMesh;
  class IMaterial;

  /**
   * @brief Interface for mesh factory classes.
   *
   * Provides an abstract interface for creating mesh instances. Implementations should
   * provide concrete mesh creation logic.
   */
  class IMeshFactory
  {
  public:
    virtual ~IMeshFactory() = default;

    /**
     * @brief Creates a mesh instance.
     *
     * @return Shared pointer to the created mesh instance. nullptr if creation fails.
     */
    virtual SharedPtr<IMesh> createMesh() = 0;

  protected:
    IMeshFactory() = default;
  };
}
