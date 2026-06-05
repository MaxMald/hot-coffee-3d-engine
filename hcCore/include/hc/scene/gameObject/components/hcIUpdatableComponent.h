#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  /**
   * @brief Interface for components that require regular updates each frame.
   */
  class HC_CORE_EXPORT IUpdatableComponent
  {
  public:
    virtual ~IUpdatableComponent() = default;

    /**
     * @brief Called before the main update loop.
     *
     * @param deltaTime The time elapsed since the last update, in seconds.
     */
    virtual void preUpdate(float deltaTime) = 0;

    /**
     * @brief Updates the component with the given delta time.
     *
     * @param deltaTime The time elapsed since the last update, in seconds.
     */
    virtual void update(float deltaTime) = 0;

    /**
     * @brief Called after the main update loop.
     *
     * @param deltaTime The time elapsed since the last update, in seconds.
     */
    virtual void postUpdate(float deltaTime) = 0;

  protected:
    IUpdatableComponent() = default;
  };
}
