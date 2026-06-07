#include "hc/scene/skybox/hcSkybox.h"
#include "hc/graphics/cubeMap/hcICubeMap.h"
#include "hc/assets/image/hcImage.h"

namespace hc
{
  Skybox::Skybox(SharedPtr<ICubeMap> cubeMap) :
    m_cubeMap(cubeMap),
    m_rightImagePath(), m_leftImagePath(), m_topImagePath(),
    m_bottomImagePath(), m_backImagePath(), m_frontImagePath()
  {
    if (m_cubeMap == nullptr)
      throw RuntimeErrorException("Skybox initialization failed: CubeMap is null.");
  }

  Skybox::~Skybox()
  {
    destroy();
  }

  void Skybox::initialize(
    const UInt32 width,
    const UInt32 height,
    const UInt8 channels,
    const Image & right,
    const Image & left,
    const Image & top,
    const Image & bottom,
    const Image & back,
    const Image & front
  )
  {
    if (m_cubeMap == nullptr)
      throw RuntimeErrorException("Skybox initialization failed: CubeMap is null.");

    m_cubeMap->initialize(
      width, height, channels,
      right, left, top,
      bottom, back, front
    );

    m_rightImagePath = right.getPath();
    m_leftImagePath = left.getPath();
    m_topImagePath = top.getPath();
    m_bottomImagePath = bottom.getPath();
    m_backImagePath = back.getPath();
    m_frontImagePath = front.getPath();
  }

  bool Skybox::isValid() const
  {
    return m_cubeMap != nullptr && m_cubeMap->isValid();
  }

  const ICubeMap& Skybox::getCubeMap() const
  {
    if (m_cubeMap == nullptr)
      throw RuntimeErrorException("Skybox cube map is null.");
    return *m_cubeMap;
  }

  const Path& Skybox::getRightImagePath() const
  {
    return m_rightImagePath;
  }

  const Path& Skybox::getLeftImagePath() const
  {
    return m_leftImagePath;
  }

  const Path& Skybox::getTopImagePath() const
  {
    return m_topImagePath;
  }

  const Path& Skybox::getBottomImagePath() const
  {
    return m_bottomImagePath;
  }

  const Path& Skybox::getBackImagePath() const
  {
    return m_backImagePath;
  }

  const Path& Skybox::getFrontImagePath() const
  {
    return m_frontImagePath;
  }

  void Skybox::destroy()
  {
    if (m_cubeMap != nullptr)
      m_cubeMap->destroy();
  }
}
