#include "hc/scene/skybox/hcSkybox.h"
#include "hc/graphics/resource/cubeMap/hcICubeMap.h"
#include "hc/assets/image/hcImage.h"

namespace hc
{
  Skybox::Skybox() : sourcePath(), m_cubeMap(nullptr)
  {}

  Skybox::~Skybox()
  {
    destroy();
  }

  void Skybox::initialize(SharedPtr<ICubeMap> cubeMap, const Path& _sourcePath)
  {
    if (cubeMap == nullptr)
      throw RuntimeErrorException("CubeMap is undefined.");
    m_cubeMap = cubeMap;
    sourcePath = _sourcePath;
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

  void Skybox::clear()
  {
    m_cubeMap.reset();
    sourcePath.clear();
  }

  void Skybox::destroy()
  {
    m_cubeMap.reset();
    sourcePath.clear();
  }
}
