#pragma once

#include <hc/graphics/resource/texture/hcTextureManager.h>
#include <hc/graphics/resource/mesh/hcMeshManager.h>
#include <hc/graphics/resource/material/hcMaterialManager.h>
#include <hc/graphics/resource/shader/hcShaderManager.h>
#include <hc/graphics/resource/shaderProgram/hcShaderProgramManager.h>
#include "hc/graphics/resource/frameBuffer/hcOpenGlGBuffer.h"
#include "hc/hcGraphicsOpenGlPrerequisites.h"
#include "hc/graphics/ubos/hcLightFrameUBO.h"

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
     * @copydoc IGraphicsManager::updateCameraRenderData
     */
    void updateCameraRenderData(const CameraRenderData& cameraRenderData) override;

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
    CameraRenderData m_currentCameraRenderData;
    LightFrameUBO m_lightFrameUBO;
    IFrameBuffer* m_customRenderTarget;

    Vector<DrawCommand> m_queueDrawCommands;
    Vector<DrawCommand> m_deferredGeometryPassCommands;
    Vector<DrawCommand> m_deferredForwardPassCommands;

    Rect<UInt32> m_viewportRect;
    OpenGlGBuffer m_gBuffer;
    SharedPtr<IShaderProgram> m_deferredLightingShaderProgram;

    polygonFillType::Type m_polygonFillType;
    renderPipelineType::Type m_renderPipelineType;

    /**
     * @copydoc IGraphicsManager::destroy
     */
    void destroy() override;

    void executeForwardPass(const Vector<DrawCommand>& drawCommands);
    void executeDeferredGeometryPass(const Vector<DrawCommand>& drawCommands);
    void executeDeferredLightingPass();
    void executeDeferredForwardPass(const Vector<DrawCommand>& drawCommands);

    void executeDrawCommand(const DrawCommand& command);
    bool isValidDrawCommand(const DrawCommand& drawCommand, String& errorMessage);

    friend class OpenGlGraphicsPlugin;
  };
}
