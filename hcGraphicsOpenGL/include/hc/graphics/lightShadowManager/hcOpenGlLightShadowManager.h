#pragma once

#include <hc/graphics/lightShadowManager/hcALightShadowManager.h>
#include "hc/hcGraphicsOpenGlPrerequisites.h"
#include "hc/graphics/ubos/hcUniformBufferObject.h"

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
     * @copydoc ALightShadowManager::destroy()
     */
    void destroy() override;
    
    /**
     * @brief Initializes the OpenGL light shadow manager by creating and binding the
     * uniform buffer object (UBO) for light shadow frame data.
     *
     * @param bindingPoint The binding point index to which the UBO will be bound.
     */
    void initialize(UInt32 bindingPoint);

  private:
    LightShadowFrameUBO m_lightShadowUBO;
    Vector<DrawCommand> m_shadowDrawCommands;

    /**
     * @copydoc ALightShadowManager::generateDirectionalLightShadowTexture()
     */
    Int32 generateDirectionalLightShadowTexture(
      Vector3f lightPosition,
      Matrix4 lightViewProjectionMatrix,
      const SceneGraph& sceneGraph
    ) override;
  };
}
