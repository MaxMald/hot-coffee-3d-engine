#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  struct RenderContext;

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
     * @brief Draw the object with a parent transformation applied.
     *
     * @param renderContext The rendering context containing transformation data.
     */
    virtual void draw(const RenderContext& renderContext) = 0;

  protected:
    IDrawable() = default;
  };
}
