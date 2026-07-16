#include "hc/graphics/lightShadowManager/hcOpenGlLightShadowManager.h"

namespace hc
{
  OpenGlLightShadowManager::OpenGlLightShadowManager() :
    m_lightShadowUBO(),
    m_shadowDrawCommands()
  {}

  OpenGlLightShadowManager::~OpenGlLightShadowManager()
  {
    destroy();
  }

  void OpenGlLightShadowManager::uploadShadowDataToGPU()
  {
    m_lightShadowUBO.upload(m_lightShadowFrameData);
  }

  void OpenGlLightShadowManager::destroy()
  {
    m_lightShadowUBO.destroy();
  }

  void OpenGlLightShadowManager::initialize(UInt32 bindingPoint)
  {
    m_lightShadowUBO.initialize(LightShadowFrameData{});
    m_lightShadowUBO.bindBase(bindingPoint);
  }

  Int32 OpenGlLightShadowManager::generateDirectionalLightShadowTexture(
    Vector3f lightPosition,
    Matrix4 lightViewProjectionMatrix,
    const SceneGraph& sceneGraph
  )
  {
    RenderContext renderContext;
    renderContext.cameraWorldPosition = lightPosition;
    renderContext.transform = Matrix4::Identity();
    renderContext.modelPosition = Vector3f(0.0f, 0.0f, 0.0f);

    m_shadowDrawCommands.clear();
    sceneGraph.draw(renderContext, m_shadowDrawCommands);



    return Int32();
  }
}
