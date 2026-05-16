#pragma once

#include <hc/graphics/resource/texture/hcTextureManager.h>
#include <hc/graphics/resource/mesh/hcMeshManager.h>
#include <hc/graphics/resource/material/hcMaterialManager.h>
#include <hc/graphics/resource/shader/hcShaderManager.h>
#include <hc/graphics/resource/shaderProgram/hcShaderProgramManager.h>
#include "hc/hcGraphicsOpenGlPrerequisites.h"

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
     * @copydoc IGraphicsManager::getGraphicsBackendType
     */
    graphicsBackendType::Type getGraphicsBackendType() const override;

    /**
     * @copydoc IGraphicsManager::beginFrame
     */
    void beginFrame() override;

    /**
     * @copydoc IGraphicsManager::draw
     */
    void draw(const DrawCommand& command) override;

    /**
     * @copydoc IGraphicsManager::executeDrawCommands
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
     * @copydoc IGraphicsManager::createFrameBuffer
     */
    FrameBufferPtr createFrameBuffer(
      UInt32 width,
      UInt32 height
    ) override;

    /**
     * @copydoc IGraphicsManager::setViewport
     */
    void setViewport(
      UInt32 x,
      UInt32 y,
      UInt32 width,
      UInt32 height
    ) override;

  private:
    IAssetManager& m_assetManager;
    IWindow& m_window;
    TextureManager m_textureManager;
    ShaderManager m_shaderManager;
    ShaderProgramManager m_shaderProgramManager;
    MaterialManager m_materialManager;
    MeshManager m_meshManager;
    Vector<DrawCommand> m_drawCommands;

    /**
     * @copydoc IGraphicsManager::initialize
     */
    void initialize() override;

    /**
     * @copydoc IGraphicsManager::destroy
     */
    void destroy() override;

    void executeDrawCommand(const DrawCommand& command);

    friend class OpenGlGraphicsPlugin;
  };
}
