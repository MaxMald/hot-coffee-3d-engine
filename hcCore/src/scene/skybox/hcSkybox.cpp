#include "hc/scene/skybox/hcSkybox.h"
#include "hc/graphics/cubeMap/hcICubeMap.h"
#include "hc/assets/image/hcImage.h"

namespace hc
{
  Skybox::Skybox() : m_cubeMap(nullptr)
  {}

  Skybox::~Skybox()
  {
    destroy();
  }

  void Skybox::initialize(SharedPtr<ICubeMap> cubeMap)
  {
    if (cubeMap == nullptr)
      throw RuntimeErrorException("CubeMap is undefined.");
    m_cubeMap = cubeMap;
  }

  bool Skybox::isValid() const
  {
    return m_cubeMap != nullptr && m_cubeMap->isValid();
  }

  bool Skybox::hasCubeMap() const
  {
    return m_cubeMap != nullptr;
  }

  const ICubeMap& Skybox::getCubeMap() const
  {
    if (m_cubeMap == nullptr)
      throw RuntimeErrorException("CubeMap is undefined.");
    return *m_cubeMap;
  }

  ICubeMap& Skybox::getCubeMap()
  {
    if (m_cubeMap == nullptr)
      throw RuntimeErrorException("CubeMap is undefined.");
    return *m_cubeMap;
  }

  void Skybox::destroy()
  {
    m_cubeMap = nullptr;
  }
}
