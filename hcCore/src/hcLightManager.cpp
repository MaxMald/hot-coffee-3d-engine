#include "hc/hcLightManager.h"
#include "hc/hcLight.h"

namespace hc
{
  LightManager::LightManager()
  {
  }

  LightManager::~LightManager()
  {
  }

  Light* LightManager::createLight(lightType::Type type)
  {
    UniquePtr<Light> light = MakeUnique<Light>(type);
    Light* lightPtr = light.get();
    m_lights.push_back(std::move(light));

    return lightPtr;
  }

  void LightManager::destroyLight(Light* light)
  {
    if (!light)
      return;

    auto it = std::find_if(
      m_lights.begin(),
      m_lights.end(),
      [light](const UniquePtr<Light>& ptr) { return ptr.get() == light; }
    );

    if (it != m_lights.end())
    {
      m_lights.erase(it);
    }
  }

  void LightManager::clear()
  {
    m_lights.clear();
  }

  const Vector<UniquePtr<Light>>& LightManager::getLights() const
  {
    return m_lights;
  }
}
