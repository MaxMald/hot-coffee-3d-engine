#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/window/hcIWindow.h"
#include "hc/graphics/resource/shader/hcShaderStageType.h"
#include "hc/graphics/hcDrawCommand.h"
#include "hc/graphics/hcIFrameBuffer.h"
#include "hc/graphics/hcGraphicsBackendType.h"
#include "hc/graphics/hcPolygonFillType.h"
#include "hc/graphics/hcRenderPipelineType.h"

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
  class IGBuffer;
  struct GraphicsSettings;
  struct CameraRenderData;
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
     * @brief Updates the camera render data for the current frame.
     *
     * @param cameraRenderData The camera render data for the current frame.
     */
    virtual void updateCameraRenderData(const CameraRenderData& cameraRenderData) = 0;

    /**
     * @brief Uploads the aggregated light data for the current frame to the GPU.
     *
     * @param lightFrameData The aggregated light data for the current frame.
     */
    virtual void uploadLightFrameData(const LightFrameData& lightFrameData) = 0;

    /**
     * @brief Issues a draw command to render graphics for the current frame.
     *
     * @param command The draw command containing rendering instructions.
     */
    virtual void draw(const DrawCommand& command) = 0;

    /**
     * @brief Executes all issued draw commands for the current frame.
     */
    virtual void executeDrawCommands() = 0;

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
