#pragma once

namespace hc
{
  class IGraphicsManager;

  /**
   * @brief Interface for drawable objects.
   *
   * Classes implementing this interface can be rendered by the engine. The draw
   * method is called with a reference to an IGraphicsManager, which provides the
   * necessary context for rendering.
   */
  class IDrawable
  {
  public:
    virtual ~IDrawable() = default;

    /**
     * @brief Draw the object using the provided graphics manager.
     * 
     * @param graphicsManager Reference to the graphics manager responsible for
     * rendering.
     */
    virtual void draw(IGraphicsManager& graphicsManager) = 0;
  };
}
