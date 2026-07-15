#include "hc/graphics/lightShadowManager/hcLightShadowManager.h"

namespace hc
{
  LightShadowManager::LightShadowManager() :
    m_lightShadowUBO()
  {}

  LightShadowManager::~LightShadowManager()
  {
    destroy();
  }

  void LightShadowManager::initialize(UInt32 bindingPoint)
  {
    m_lightShadowUBO.initialize(LightShadowFrameData{});
    m_lightShadowUBO.bindBase(bindingPoint);
  }

  void LightShadowManager::destroy()
  {
    m_lightShadowUBO.destroy();
  }
}
