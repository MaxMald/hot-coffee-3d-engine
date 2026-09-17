#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  struct RenderContext;
  struct DrawCommand;

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
     * @param outDrawCommands Vector to store draw commands.
     */
    virtual void draw(
      const RenderContext& renderContext,
      Vector<DrawCommand>& outDrawCommands
    ) const = 0;

  protected:
    IDrawable() = default;
  };
}
