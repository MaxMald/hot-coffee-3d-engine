#pragma once

#include <hc/graphics/lightShadowManager/hcILightShadowMapManager.h>
#include <hc/graphics/resource/dataBlock/hcDataBlockStructures.h>

#include "hc/hcGraphicsOpenGlPrerequisites.h"
#include "hc/graphics/resource/shaderProgram/hcOpenGlShaderProgram.h"
#include "hc/graphics/resource/frameBuffer/hcOpenGlShadowFrameBuffer.h"

namespace hc
{
  class IDataBlockManager;

  class OpenGlLightShadowMapManager : public ILightShadowMapManager
  {
  public:
    OpenGlLightShadowMapManager(IDataBlockManager& dataBlockManager);
    virtual ~OpenGlLightShadowMapManager();

    /**
     * @copydoc ILightShadowMapManager::bindShadowTexturesForReading()
     */
    void bindShadowTexturesForReading() override;

    /**
     * @copydoc ILightShadowMapManager::generateDirectionalLightShadowTexture()
     */
    Int32 generateDirectionalLightShadowTexture(
      Vector3f lightPosition,
      Matrix4 lightViewProjectionMatrix,
      const SceneGraph& sceneGraph
    ) override;

    /**
     * @copydoc ILightShadowMapManager::generateSpotLightShadowTexture()
     */
    Int32 generateSpotLightShadowTexture(
      Vector3f lightPosition,
      Matrix4 lightViewProjectionMatrix,
      const SceneGraph& sceneGraph
    ) override;

    /**
     * @copydoc ILightShadowMapManager::clear()
     */
    void clear() override;

    /**
     * @copydoc ILightShadowMapManager::hasReachedMaxDirectionalLightShadows()
     */
    bool hasReachedMaxDirectionalLightShadows() const override;

    /**
     * @copydoc ILightShadowMapManager::hasReachedMaxSpotLightShadows()
     */
    bool hasReachedMaxSpotLightShadows() const override;

    /**
     * @copydoc ILightShadowMapManager::destroy()
     */
    void destroy() override;

    /**
     * @brief Initializes the OpenGL light shadow manager by creating and binding the
     * uniform buffer object (UBO) for light shadow frame data.
     *
     * @param shaderProgramManager Reference to the shader program manager.
     * @param directionalShadowMapSize The size of the shadow map texture to be created
     * for directional lights.
     * @param spotShadowMapSize The size of the shadow map texture to be created for spot
     * lights.
     */
    void initialize(
      IShaderProgramManager& shaderProgramManager,
      UInt32 directionalShadowMapSize = 1024,
      UInt32 spotShadowMapSize = 1024
    );

  private:
    Vector<DrawCommand> m_shadowDrawCommands;
    SharedPtr<IShaderProgram> m_shadowMapShaderProgram;
    OpenGlShadowFrameBuffer m_directionalShadowFrameBuffer;
    OpenGlShadowFrameBuffer m_spotShadowFrameBuffer;
    UInt32 m_currentDirectionalShadowMapLayer;
    UInt32 m_currentSpotShadowMapLayer;
    IDataBlockManager& m_dataBlockManager;
  };
}
