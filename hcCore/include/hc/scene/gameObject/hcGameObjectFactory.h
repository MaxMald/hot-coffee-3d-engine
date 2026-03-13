#pragma once

#include "hc/scene/gameObject/hcIGameObjectFactory.h"

namespace hc
{
  class ComponentFactoriesManager;
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
    /**
     * @brief Constructs a GameObjectFactory with the given
     * ComponentFactoriesManager.
     *
     * @param componentFactoriesManager Unique pointer to the
     * ComponentFactoriesManager to use for creating components in GameObjects.
     */
    GameObjectFactory(
      UniquePtr<ComponentFactoriesManager> componentFactoriesManager
    );
    ~GameObjectFactory() override;

    /**
     * @brief Creates a new GameObject instance with the specified name.
     *
     * @param name The name to assign to the created GameObject.
     *
     * @return UniquePtr<GameObject> The created GameObject.
     */
    UniquePtr<GameObject> create(const String& name) override;

  private:
    UniquePtr<ComponentFactoriesManager> m_componentFactoriesManager;
  };
}
