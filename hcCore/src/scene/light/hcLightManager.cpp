#include "hc/scene/light/hcLightManager.h"
#include "hc/scene/light/hcLight.h"

namespace hc
{
  LightManager::LightManager() :
    m_lights()
  {
  }

  LightManager::~LightManager()
  {
  }

  void LightManager::serialize(BinaryWriter& writer) const
  {
    writer.writeSizeT(m_lights.size());
    for (const auto& pair : m_lights)
      pair.second->serialize(writer);
  }

  void LightManager::deserialize(BinaryReader& reader)
  {
    clear();

    SizeT lightCount = reader.readSizeT();
    for (SizeT i = 0; i < lightCount; ++i)
    {
      UniquePtr<Light> light = MakeUnique<Light>();
      light->deserialize(reader);
      UUID lightId = light->getUUID();
      m_lights.emplace(lightId, std::move(light));
    }
  }

  Light* LightManager::createLight(lightType::Type type)
  {
    UniquePtr<Light> light = MakeUnique<Light>(type);
    Light* lightPtr = light.get();
    m_lights.emplace(lightPtr->getUUID(), std::move(light));
    return lightPtr;
  }

  void LightManager::destroyLight(const UUID& lightId)
  {
    auto it = m_lights.find(lightId);
    if (it != m_lights.end())
    {
      m_lights.erase(it);
      return;
    }

    LogService::Error(
      "Attempted to destroy light with UUID " + lightId.toString() + ", but it does not exist."
    );
  }

  void LightManager::clear()
  {
    m_lights.clear();
  }

  void LightManager::getLights(Vector<Light*>& outLights) const
  {
    outLights.clear();
    for (const auto& pair : m_lights)
      outLights.push_back(pair.second.get());
  }
}
