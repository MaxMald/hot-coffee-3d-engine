#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/window/hcIWindow.h"
#include "hc/graphics/resource/shader/hcShaderStageType.h"

namespace hc
{
  class HotCoffeeEngine;
  class AssetManager;
  class SceneManager;
  class ITextureManager;
  class IMaterialManager;
  class IShaderManager;
  class IShaderProgramManager;
  class IMeshManager;

  /**
   * @brief Interface for graphics manager classes.
   *
   * Provides an interface for managing graphics-related subsystems and frame
   * operations.
   */
  class HC_CORE_EXPORT IGraphicsManager : public NonCopyable
  {
  public:
    virtual ~IGraphicsManager();

    /**
     * @brief Begins a new rendering frame.
     */
    virtual void beginFrame() = 0;

    /**
     * @brief Ends the current rendering frame and presents it to the given
     * window.
     *
     * @param window Reference to the window where the frame will be presented.
     */
    virtual void endFrame(IWindow&) = 0;

    /**
     * @brief Returns the texture manager.
     *
     * @return Reference to the ITextureManager instance.
     */
    virtual ITextureManager& getTextureManager() = 0;

    /**
     * @brief Returns the material manager.
     *
     * @return Reference to the IMaterialManager instance.
     */
    virtual IMaterialManager& getMaterialManager() = 0;

    /**
     * @brief Returns the shader manager.
     *
     * @return Reference to the IShaderManager instance.
     */
    virtual IShaderManager& getShaderManager() = 0;

    /**
     * @brief Returns the shader program manager.
     *
     * @return Reference to the IShaderProgramManager instance.
     */
    virtual IShaderProgramManager& getShaderProgramManager() = 0;

    /**
     * @brief Returns the mesh manager.
     *
     * @return Reference to the IMeshManager instance.
     */
    virtual IMeshManager& getMeshManager() = 0;

  protected:
    IGraphicsManager();

    /**
     * @brief Initializes the graphics manager.
     */
    virtual void initialize() = 0;

    /**
     * @brief Destroys and cleans up the graphics manager.
     */
    virtual void destroy() = 0;

    friend HotCoffeeEngine;
  };
}
