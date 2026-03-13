#include "hc/assets/materialDescriptor/hcAMaterialDescriptor.h"

namespace hc
{
  materialRenderMode::Type AMaterialDescriptor::getRenderMode() const
  {
    return m_renderMode;
  }

  void AMaterialDescriptor::setRenderMode(materialRenderMode::Type renderMode)
  {
    m_renderMode = renderMode;
  }

  void AMaterialDescriptor::setAlphaCutoutThreshold(float threshold)
  {
    m_alphaCutoutThreshold = threshold;
  }

  float AMaterialDescriptor::getAlphaCutoutThreshold() const
  {
    return m_alphaCutoutThreshold;
  }

  bool AMaterialDescriptor::isDoubleSided() const
  {
    return m_doubleSided;
  }

  void AMaterialDescriptor::setDoubleSided(bool doubleSided)
  {
    m_doubleSided = doubleSided;
  }

  AMaterialDescriptor::AMaterialDescriptor(
    const Path& path,
    materialRenderMode::Type renderMode
  ) :
    Asset(path),
    m_renderMode(renderMode),
    m_alphaCutoutThreshold(0.5f),
    m_doubleSided(false)
  {
  }
}
