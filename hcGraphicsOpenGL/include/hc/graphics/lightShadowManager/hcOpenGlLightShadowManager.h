#pragma once

#include <hc/graphics/lightShadowManager/hcALightShadowManager.h>
#include "hc/hcGraphicsOpenGlPrerequisites.h"
#include "hc/graphics/ubos/hcUniformBufferObject.h"
#include "hc/graphics/resource/shaderProgram/hcOpenGlShaderProgram.h"
#include "hc/graphics/resource/frameBuffer/hcOpenGlShadowFrameBuffer.h"

namespace hc
{
  class OpenGlLightShadowManager : public ALightShadowManager
  {
  public:
    OpenGlLightShadowManager();
    virtual ~OpenGlLightShadowManager();

    /**
     * @copydoc ALightShadowManager::uploadShadowDataToGPU()
     */
    void uploadShadowDataToGPU() override;

    /**
     * @copydoc ALightShadowManager::bindShadowTexturesForReading()
     */
    void bindShadowTexturesForReading(
      UInt8 directionalTextureArrayUnit,
      UInt8 spotTextureArrayUnit
    ) override;

    /**
     * @copydoc ALightShadowManager::destroy()
     */
    void destroy() override;

    /**
     * @brief Initializes the OpenGL light shadow manager by creating and binding the
     * uniform buffer object (UBO) for light shadow frame data.
     *
     * @param bindingPoint The binding point index to which the UBO will be bound.
     * @param shaderProgramManager Reference to the shader program manager.
     * @param directionalShadowMapSize The size of the shadow map texture to be created
     * for directional lights.
     * @param spotShadowMapSize The size of the shadow map texture to be created for spot
     * lights.
     */
    void initialize(
      UInt32 bindingPoint,
      IShaderProgramManager& shaderProgramManager,
      UInt32 directionalShadowMapSize = 1024,
      UInt32 spotShadowMapSize = 1024
    );

  private:
    LightShadowFrameUBO m_lightShadowUBO;
    Vector<DrawCommand> m_shadowDrawCommands;
    SharedPtr<IShaderProgram> m_shadowMapShaderProgram;
    OpenGlShadowFrameBuffer m_directionalShadowFrameBuffer;
    OpenGlShadowFrameBuffer m_spotShadowFrameBuffer;
    UInt32 m_currentDirectionalShadowMapLayer;
    UInt32 m_currentSpotShadowMapLayer;

    /**
     * @copydoc ALightShadowManager::generateDirectionalLightShadowTexture()
     */
    Int32 generateDirectionalLightShadowTexture(
      Vector3f lightPosition,
      Matrix4 lightViewProjectionMatrix,
      const SceneGraph& sceneGraph
    ) override;

    /**
     * @copydoc ALightShadowManager::generateSpotLightShadowTexture()
     */
    Int32 generateSpotLightShadowTexture(
      Vector3f lightPosition,
      Matrix4 lightViewProjectionMatrix,
      const SceneGraph& sceneGraph
    ) override;

    /**
     * @copydoc ALightShadowManager::onClear()
     */
    void onClear() override;
  };
}
