#pragma once

#include "hc/hcIGameObjectFactory.h"

namespace hc
{
  class GameObject;

  /**
   * @brief Concrete factory for creating GameObject instances.
   *
   * Uses a reference to ComponentFactoriesManager to ensure all created
   * GameObjects are properly initialized with component factory support.
   */
  class GameObjectFactory : public IGameObjectFactory
  {
  public:
    GameObjectFactory();
    ~GameObjectFactory() override = default;

    /**
     * @brief Creates a new GameObject instance with the specified name.
     *
     * @param name The name to assign to the created GameObject.
     *
     * @return UniquePtr<GameObject> The created GameObject.
     */
    UniquePtr<GameObject> create(const String& name) override;
  };
}
