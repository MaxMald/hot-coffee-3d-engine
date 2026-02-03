#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  class IGraphicsManager;

  /**
   * @brief Interface for drawable objects.
   *
   * Classes implementing this interface can be rendered by the engine.
   */
  class HC_CORE_EXPORT IDrawable
  {
  public:
    virtual ~IDrawable() = default;

    /**
     * @brief Draw the object using the provided graphics manager.
     */
    virtual void draw() = 0;

    /**
     * @brief Draw the object with a parent transformation applied.
     * 
     * @param parentTransform The transformation of the parent object.
     */
    virtual void draw(const Transform& parentTransform) = 0;

  protected:
    IDrawable() = default;
  };
}
