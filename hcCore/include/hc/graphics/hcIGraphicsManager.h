#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/window/hcIWindow.h"
#include "hc/graphics/hcGraphicsCommons.h"
#include "hc/graphics/resource/frameBuffer/hcIFrameBuffer.h"
#include "hc/graphics/hcDrawCommand.h"

namespace hc
{
  class HotCoffeeEngine;
  class AssetManager;
  class SceneManager;
  class ITextureManager;
  class IMaterialManager;
  class IShaderManager;
  class IShaderProgramManager;
  class IDataBlockManager;
  class IMeshManager;
  class IGBuffer;
  class ICubeMap;
  class ILightShadowMapManager;
  struct GraphicsSettings;
  struct CameraFrameData;
  struct LightFrameData;

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
     * @brief Initializes the graphics manager with the given viewport dimensions.
     *
     * @param graphicsSettings The graphics settings to use for initialization.
     * @param viewportRect The rectangle defining the initial viewport dimensions.
     */
    virtual void initialize(
      const GraphicsSettings& graphicsSettings,
      const Rect<UInt32>& viewportRect
    ) = 0;

    /**
     * @brief Gets the type of graphics backend used by this graphics manager.
     *
     * @return The graphics backend type.
     */
    virtual graphicsBackendType::Type getGraphicsBackendType() const = 0;

    /**
     * @brief Begins a new rendering frame.
     */
    virtual void beginFrame() = 0;

    /**
     * @brief Sets the render target for all subsequent draw calls.
     *
     * The specified framebuffer becomes the active render target until
     * changed. If not set, rendering defaults to the window's framebuffer.
     *
     * @param frameBuffer The framebuffer to render to. Pass nullptr to revert
     * to the window's default framebuffer.
     */
    virtual void setRenderTarget(IFrameBuffer* frameBuffer) = 0;

    /**
     * @brief Gets the current render target framebuffer.
     *
     * @return The current render target, or nullptr if rendering to the
     * window's default framebuffer.
     */
    virtual IFrameBuffer* getRenderTarget() const = 0;

    /**
     * @brief Sets the skybox cube map to be used for rendering the skybox.
     *
     * @param skyboxCubeMap Pointer to the cube map texture to use as the skybox. Pass
     * nullptr to disable skybox rendering.
     */
    virtual void setSkybox(ICubeMap* skyboxCubeMap) = 0;

    /**
     * @brief Queues a draw command to render graphics for the current frame.
     *
     * @param command The draw command containing rendering instructions.
     */
    virtual void queueDrawCommand(const DrawCommand& command) = 0;

    /**
     * @brief Executes all queued draw commands for the current frame.
     */
    virtual void executeDrawCommands() = 0;

    /**
     * @brief Clears all queued draw commands for the current frame.
     */
    virtual void clearDrawCommands() = 0;

    /**
     * @brief Gets a reference to the draw command queue for the current frame.
     *
     * @return Reference to the vector of draw commands.
     */
    virtual Vector<DrawCommand>& getDrawCommandQueue() = 0;

    /**
     * @brief Gets a const reference to the draw command queue for the current frame.
     *
     * @return Const reference to the vector of draw commands.
     */
    virtual const Vector<DrawCommand>& getDrawCommandQueue() const = 0;

    /**
     * @brief Ends the current rendering frame and presents it to the given
     * window.
     *
     * @param window Reference to the window where the frame will be presented.
     */
    virtual void endFrame(IWindow&) = 0;

    /**
     * @brief Sets the polygon fill type for rendering.
     *
     * @param fillType The polygon fill type to set.
     */
    virtual void setPolygonFillType(polygonFillType::Type fillType) = 0;

    /**
     * @brief Gets the current polygon fill type used for rendering.
     *
     * @return The current polygon fill type.
     */
    virtual polygonFillType::Type getPolygonFillType() const = 0;

    /**
     * @brief Sets the render pipeline type for rendering (e.g., forward or deferred hybrid).
     *
     * @param renderPipelineType The render pipeline type to set.
     */
    virtual void setRenderPipelineType(renderPipelineType::Type renderPipelineType) = 0;

    /**
     * @brief Gets the current render pipeline type used for rendering.
     *
     * @return The current render pipeline type.
     */
    virtual renderPipelineType::Type getRenderPipelineType() const = 0;

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

    /**
     * @brief Returns the light shadow map manager.
     *
     * @return Reference to the ILightShadowMapManager instance.
     */
    virtual ILightShadowMapManager& getLightShadowMapManager() = 0;

    /**
     * @brief Returns the data block manager.
     *
     * @return Reference to the IDataBlockManager instance.
     */
    virtual IDataBlockManager& getDataBlockManager() = 0;

    /**
     * @brief Returns the geometry buffer used for deferred rendering.
     *
     * @return Reference to the IGBuffer instance.
     */
    virtual IGBuffer& getGBuffer() = 0;

    /**
     * @brief Creates a framebuffer with the specified dimensions.
     *
     * The returned smart pointer will automatically handle cleanup across DLL
     * boundaries.
     *
     * @param width The width in pixels.
     * @param height The height in pixels.
     *
     * @return Smart pointer to the created framebuffer.
     */
    virtual FrameBufferPtr createFrameBuffer(
      UInt32 width,
      UInt32 height
    ) = 0;

    /**
     * @brief Creates a cube map texture.
     *
     * @return Smart pointer to the created cube map texture.
     */
    virtual SharedPtr<ICubeMap> createCubeMap() = 0;

    /**
     * @brief Sets the rendering viewport dimensions.
     *
     * @param viewportRect A Rect structure defining the viewport's position and size in
     * pixels.
     */
    virtual void setViewport(const Rect<UInt32>& viewportRect) = 0;

    /**
     * @brief Gets the current viewport dimensions.
     *
     * @return A Rect structure containing the viewport's position and size.
     */
    virtual Rect<UInt32> getViewportRect() const = 0;

  protected:
    IGraphicsManager();

    /**
     * @brief Destroys and cleans up the graphics manager.
     */
    virtual void destroy() = 0;

    friend HotCoffeeEngine;
  };
}
