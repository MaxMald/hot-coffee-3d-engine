#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  class GameObject;

  /**
   * @brief Interface for factories that create GameObject instances.
   */
  class IGameObjectFactory
  {
  public:
    virtual ~IGameObjectFactory() = default;

    /**
     * @brief Creates a new GameObject instance with the specified name.
     *
     * @param name The name of the GameObject.
     *
     * @return UniquePtr<GameObject> The created GameObject.
     */
    virtual UniquePtr<GameObject> create(const String& name) = 0;

  protected:
    IGameObjectFactory() = default;
  };
}
