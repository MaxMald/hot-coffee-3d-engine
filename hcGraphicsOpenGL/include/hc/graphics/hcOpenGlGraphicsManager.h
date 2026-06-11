#pragma once

#include "hc/hcGraphicsOpenGlPrerequisites.h"
#include <hc/graphics/resource/texture/hcTextureManager.h>
#include <hc/graphics/resource/mesh/hcMeshManager.h>
#include <hc/graphics/resource/material/hcMaterialManager.h>
#include <hc/graphics/resource/shader/hcShaderManager.h>
#include <hc/graphics/resource/shaderProgram/hcShaderProgramManager.h>
#include "hc/graphics/frameRenderer/hcFrameRenderer.h"

namespace hc
{
  class HC_GRAPHICS_OPENGL_EXPORT OpenGlGraphicsManager :
    public IGraphicsManager
  {
  public:
    OpenGlGraphicsManager(
      IWindow& window,
      IAssetManager& assetManager
    );
    virtual ~OpenGlGraphicsManager();

    /**
     * @copydoc IGraphicsManager::initialize
     */
    void initialize(
      const GraphicsSettings& graphicsSettings,
      const Rect<UInt32>& viewportRect
    ) override;

    /**
     * @copydoc IGraphicsManager::getGraphicsBackendType
     */
    graphicsBackendType::Type getGraphicsBackendType() const override;

    /**
     * @copydoc IGraphicsManager::beginFrame
     */
    void beginFrame() override;

    /**
     * @copydoc IGraphicsManager::uploadCameraFrameData
     */
    void uploadCameraFrameData(const CameraFrameData& cameraFrameData) override;

    /**
     * @copydoc IGraphicsManager::uploadLightFrameData
     */
    void uploadLightFrameData(const LightFrameData& lightFrameData) override;

    /**
     * @copydoc IGraphicsManager::setRenderTarget
     */
    void setRenderTarget(IFrameBuffer* frameBuffer) override;

    /**
     * @copydoc IGraphicsManager::getRenderTarget
     */
    IFrameBuffer* getRenderTarget() const override;

    /**
     * @copydoc IGraphicsManager::setSkybox
     */
    void setSkybox(ICubeMap* skyboxCubeMap) override;

    /**
     * @copydoc IGraphicsManager::draw
     */
    void draw(const DrawCommand& command) override;

    /**
     * @copydoc IGraphicsManager::executeDrawCommands()
     */
    void executeDrawCommands() override;

    /**
     * @copydoc IGraphicsManager::endFrame
     */
    void endFrame(IWindow&) override;

    /**
     * @copydoc IGraphicsManager::setPolygonFillType
     */
    void setPolygonFillType(polygonFillType::Type fillType) override;

    /**
     * @copydoc IGraphicsManager::getPolygonFillType
     */
    polygonFillType::Type getPolygonFillType() const override;

    /**
     * @copydoc IGraphicsManager::setRenderPipelineType
     */
    void setRenderPipelineType(renderPipelineType::Type renderPipelineType) override;

    /**
     * @copydoc IGraphicsManager::getRenderPipelineType
     */
    renderPipelineType::Type getRenderPipelineType() const override;

    /**
     * @copydoc IGraphicsManager::getTextureManager
     */
    ITextureManager& getTextureManager() override;

    /**
     * @copydoc IGraphicsManager::getMaterialManager
     */
    IMaterialManager& getMaterialManager() override;

    /**
     * @copydoc IGraphicsManager::getShaderManager
     */
    IShaderManager& getShaderManager() override;

    /**
     * @copydoc IGraphicsManager::getShaderProgramManager
     */
    IShaderProgramManager& getShaderProgramManager() override;

    /**
     * @copydoc IGraphicsManager::getMeshManager
     */
    IMeshManager& getMeshManager() override;

    /**
     * @copydoc IGraphicsManager::getGBuffer
     */
    IGBuffer& getGBuffer() override;

    /**
     * @copydoc IGraphicsManager::createFrameBuffer
     */
    FrameBufferPtr createFrameBuffer(
      UInt32 width,
      UInt32 height
    ) override;

    /**
     * @copydoc IGraphicsManager::createCubeMap
     */
    SharedPtr<ICubeMap> createCubeMap() override;

    /**
     * @copydoc IGraphicsManager::setViewport
     */
    void setViewport(const Rect<UInt32>& viewportRect) override;

    /**
     * @copydoc IGraphicsManager::getViewportRect
     */
    Rect<UInt32> getViewportRect() const override;

  private:
    IAssetManager& m_assetManager;
    IWindow& m_window;
    TextureManager m_textureManager;
    ShaderManager m_shaderManager;
    ShaderProgramManager m_shaderProgramManager;
    MaterialManager m_materialManager;
    MeshManager m_meshManager;
    Rect<UInt32> m_viewportRect;
    FrameRenderer m_frameRenderer;
    polygonFillType::Type m_polygonFillType;

    /**
     * @copydoc IGraphicsManager::destroy
     */
    void destroy() override;

    friend class OpenGlGraphicsPlugin;
  };
}
